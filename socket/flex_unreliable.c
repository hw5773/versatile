#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/skbuff.h>
#include <linux/scatterlist.h>
#include <linux/splice.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/random.h>
#include <linux/bootmem.h>
#include <linux/highmem.h>
#include <linux/swap.h>
#include <linux/cache.h>
#include <linux/err.h>
#include <linux/crypto.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/netdevice.h>

#include "flex_sock.h"
#include "flex_unreliable.h"
#include "flex_dev_types.h"

struct u_table u_table __read_mostly;

void __init flex_unreliable_init(void)
{
  FLEX_LOG("Start to initialize the unreliable one");
  u_table_init(&u_table, "Unreliable");
}

void __exit flex_unreliable_exit(void)
{
  FLEX_LOG("Start to exit the unreliable one");
  u_table_exit(&u_table);
}

void u_table_init(struct u_table *table, const char *name)
{
  int i;

  FLEX_LOG("Start to initialize the Unreliable Table");
  table->hash = kmalloc(MIN_UTABLE_SIZE * 2 * sizeof(struct u_hslot), GFP_KERNEL);
  if (!(table->hash))
  {
    FLEX_LOG("Initialize Unreliable Socket Hash Table Failure");
    panic(name);
  }
  table->log = ilog2(MIN_UTABLE_SIZE);
  table->mask = MIN_UTABLE_SIZE - 1;

  for (i=0; i<= table->mask; i++)
  {
    INIT_HLIST_NULLS_HEAD(&table->hash[i].head, i);
    table->hash[i].count = 0;
    spin_lock_init(&table->hash[i].lock);
  }
  FLEX_LOG("Allocate the table Succeed");
}

void u_table_exit(struct u_table *table)
{
  kfree(table->hash);
}

struct sock *u_table_lookup_skb(flexid_t id, struct u_table *table)
{
  int err;
  struct sock *sk, *result;
  struct flex_sock *flex;
  struct u_hslot *hslot;
  struct hlist_nulls_node *node;
  unsigned int slot;

  slot = hash_fn(id, table->mask);
  hslot = &table->hash[slot];

  result = NULL;
  rcu_read_lock();
  sk_nulls_for_each_rcu(sk, node, &hslot->head)
  {
    flex = flex_sk(sk);
    if (flex->message == FLEX_INTEREST)
    {
      if (strncmp(flex->dst.identity, id.identity, sizeof(flexid_t)) == 0)
        result = sk;
    }
    else if (flex->message == FLEX_DATA)
    {
      if (strncmp(flex->src.identity, id.identity, sizeof(flexid_t)) == 0)
        result = sk;
    }
  }
  rcu_read_unlock();

  if (!result)
  {
    FLEX_LOG("Fail to find the appropriate socket.");
    err = -NO_SOCK;
    goto out;
  }

  FLEX_LOG("Find the appropriate socket.");

  return result;

out:
  return NULL;
}

int flex_unreliable_connect(struct socket *sock, struct sockaddr *taddr, int addr_len, int flags)
{
  int i;
  struct sock *sk, *test;
  struct flex_sock *flex;
  struct sockaddr_flex *tinfo;
  struct u_hslot *hslot;
  unsigned int slot;
  struct u_table *table;

	FLEX_LOG("Unreliable Connect");

  FLEX_LOG("Bind the Socket with the Target ID");

  sk = sock->sk;
  flex = flex_sk(sk);
  tinfo = target_info(taddr);

  memcpy(&(flex->src), &(tinfo->sid), sizeof(flexid_t));
  memcpy(&(flex->dst), &(tinfo->tid), sizeof(flexid_t));
  flex->message = tinfo->message;

  FLEX_LOG("Set the next hop to the Socket");

  flex->addr_type = tinfo->addr_type;

  for (i=0; i<tinfo->addr_len; i++)
    flex->next_hop[i] = tinfo->next_hop[i];

  table = &u_table;

  slot = hash_fn(flex->dst, table->mask);
  hslot = &table->hash[slot];
  sk_nulls_add_node_rcu(sk, &hslot->head);
  hslot->count++;

  FLEX_LOG("Add the Socket Complete");

	return SUCCESS;
}

unsigned int flex_unreliable_poll(struct file *file, struct socket *sock, poll_table *wait)
{
	FLEX_LOG("Unreliable Polling");
	return -1;
}

int flex_unreliable_sendmsg(struct socket *sock, struct msghdr *msg, size_t size)
{
  int i, err;
  struct net_device *dev;
  struct sock *sk;
  struct flex_sock *flex;
  struct sk_buff *skb;
  struct uflexhdr *flexh;
  unsigned char *content;
  DECLARE_SOCKADDR(struct sockaddr_flex *, usflex, msg->msg_name);

  FLEX_LOG("Send the unreliable message");

  sk = sock->sk;
  flex = flex_sk(sk);

  FLEX_LOG("Find the device to send");
  for (i=0; i<MAX_DEV_TYPES; i++)
  { 
    dev = dev_getfirstbyhwtype(&init_net, dev_type[i]);

    if (dev)
      break;
  }

  if (!dev)
  {
    FLEX_LOG("Error: No device to send");
    err = -NO_DEV;
    goto out;
  }
  else
    FLEX_LOG1s("Device Name", netdev_name(dev));

  skb = alloc_skb(sizeof(uflexhdr_t) + LL_RESERVED_SPACE(dev), GFP_ATOMIC);

  if (!skb)
  {
    FLEX_LOG("Allocate Socket Buffer Failed");
    err = -ERROR_MALLOC;
    goto out;
  }

  FLEX_LOG("Initialize the Flex Header");

  skb_reserve(skb, LL_RESERVED_SPACE(dev));
  skb->dev = dev;
  skb->protocol = htons(ETH_P_FLEX);

  flexh = (struct uflexhdr *)skb_put(skb, sizeof(struct uflexhdr));
  flexh->common.version = FLEX_1_0;

  FLEX_LOG("Find the message type");
  
  FLEX_LOG1x("Message Type", flex->message);

  switch (flex->message)
  {
    case FLEX_INTEREST:
      flexh->common.packet_type = FLEX_INTEREST;
      FLEX_LOG("This is INTEREST message");
      break;
    case FLEX_DATA:
      flexh->common.packet_type = FLEX_DATA;
      content = (unsigned char *)kmalloc(size, GFP_ATOMIC);
      memcpy_from_msg(content, msg, size);
      FLEX_LOG("This is DATA message");
      break;
    case FLEX_DATA_ACK:
      FLEX_LOG("Error: Unreliable communication doesn't support ACK");
      err = -WRONG_MESSAGE_TYPE;
      goto out;
    default:
      FLEX_LOG("Error: We only support data plane message yet");
      err = -WRONG_MESSAGE_TYPE;
      goto out;
  }

  flexh->common.hash_type = SHA1;
  flexh->common.hop_limit = DEFAULT_HOP_LIMIT;
  flexh->common.header_len = htons(UNRELIABLE_HEADER_LEN);
  flexh->common.check = htons(0x1234);
  flexh->common.packet_id = htons(0x7777);
  flexh->common.frag_off = htons(FLEX_PTC_UNRELIABLE | FLEX_DF | 0x365);
  memset(flexh->sflex_id, '1', FLEX_ID_LENGTH);
  memcpy(flexh->dflex_id, &(flex->dst), flex->dst.length);
  flexh->packet_len = htons(UNRELIABLE_HEADER_LEN);

  if (dev_hard_header(skb, dev, ETH_P_FLEX, flex->next_hop, dev->dev_addr, skb->len) < 0)
  {
    FLEX_LOG("Make Header Frame Failed");
    err = -WRONG_HEADER;
    goto out;
  }

  FLEX_LOG("Make Header Frame Success");
  dev_queue_xmit(skb);
  FLEX_LOG("Send the Frame");

	return SUCCESS;

out:
  return err;
}

int flex_unreliable_recvmsg(struct socket *sock, struct msghdr *msg, size_t size, int flags)
{
	FLEX_LOG("Receive the unreliable message");
	return -1;
}

int flex_unreliable_release(struct socket *sock)
{
  struct sock *sk;
  struct flex_sock *flex;
  struct u_table *table;
  struct u_hslot *hslot;
  unsigned int slot;

	FLEX_LOG("Release the socket internally");

  sk = sock->sk;

  if (!sk)
    return 0;

  flex = flex_sk(sk);
  table = &u_table;

  if (flex->message == FLEX_INTEREST)
    slot = hash_fn(flex->dst, table->mask);
  else if (flex->message == FLEX_DATA)
    slot = hash_fn(flex->src, table->mask);
  hslot = &table->hash[slot];

  spin_lock_bh(&hslot->lock);
  if (sk_nulls_del_node_init_rcu(sk))
    hslot->count--;
  spin_unlock_bh(&hslot->lock);

	sock_set_flag(sk, SOCK_DEAD);
	sock_set_flag(sk, SOCK_DESTROY);

	FLEX_LOG("Invoke sock_orphan()");
	sock_orphan(sk);
	FLEX_LOG("Invoke release_sock()");
	release_sock(sk);
	FLEX_LOG("Invoke sock_put()");
	sock_put(sk);

  sock->sk = NULL;

	return 0;
}
