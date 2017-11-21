#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/skbuff.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/err.h>
#include <linux/time.h>
#include <linux/netdevice.h>

#include "flex_sock.h"
#include "flex_dev_types.h"
#include "flex_idtable.h"

#include <linux/list.h>
#include <linux/rculist.h>

#include <flex/flex_const.h>

int urepo_sock;

/**
 * @brief Initialize the Flex ID network
 */
void __init flex_unreliable_init(void)
{
  FLEX_LOG("Start to initialize the unreliable one");
  id_table_init(&id_table, "Unreliable");
  FLEX_LOG1p("id_table", &id_table);
}

/**
 * @brief Destruct the Flex ID network
 */
void __exit flex_unreliable_exit(void)
{
  FLEX_LOG("Start to exit the unreliable one");
  id_table_exit(&id_table);
}

/**
 * @brief Connect funtion set the source Flex ID, the destination Flex ID, the message type, and the next hop to the socket
 * @param sock Socket to be set
 * @param taddr The data structure including the above information
 * @param addr_len The length of the data structure
 * @param flags Not used
 * @return Error code
 */
int flex_unreliable_connect(struct socket *sock, struct sockaddr *taddr, int addr_len, int flags)
{
  int i, err;
  flexid_t *id;
  struct sock *sk;
  struct flex_sock *flex;
  struct sockaddr_flex *id_info;
  FLEX_LOG("Unreliable Connect");

  FLEX_LOG("Bind the Socket with the Target ID");

  id_info = target_info(taddr);
  id = &id_info->tid;
  sk = sock->sk;
  flex = flex_sk(sk);

  flex->src = id_info->sid;
  flex->dst = id_info->tid;
  flex->message = id_info->message;

  FLEX_LOG("Set the next hop to the Socket");

  flex->addr_type = id_info->addr_type;

  for (i=0; i<(id_info->addr_len); i++)
    flex->next_hop[i] = id_info->next_hop[i];

  if ((err = add_id_to_table(id, sk, &id_table)) < 0) goto out;
  FLEX_LOG("Add ID to socket map succeed");

  return SUCCESS;

out:
  return err;
}

unsigned int flex_unreliable_poll(struct file *file, struct socket *sock, poll_table *wait)
{
	FLEX_LOG("Unreliable Polling");
	return -1;
}

/**
 * @brief Send the message via the socket
 * @param sock The data structure of the socket
 * @param msg The message to be sent
 * @param size The size of the message
 * @return Error code
 */
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

  flexh->common.hash_type = htons(FLEX_SHA1);
  flexh->common.hop_limit = htons(DEFAULT_HOP_LIMIT);
  flexh->common.header_len = htons(UNRELIABLE_HEADER_LEN);
  flexh->common.check = htons(0x1234);
  flexh->common.packet_id = htons(0x7777);
  flexh->common.frag_off = htons(FLEX_PTC_UNRELIABLE | FLEX_DF | 0x365);
  memset(flexh->sflex_id, '1', FLEX_ID_LENGTH);
  memcpy(flexh->dflex_id, &flex->dst, flex->dst.length);
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

/**
 * @brief The function to receive the message by the socket
 * @param sock The socket received message
 * @param msg The message received
 * @param size The length of the message
 * @param flags Not used
 * @return Error code
 */
int flex_unreliable_recvmsg(struct socket *sock, struct msghdr *msg, size_t size, int flags)
{
  int err, off, peeked, hlen, plen, bytes;
  struct sock *sk = sock->sk;
  struct sockaddr_flex *faddr = (struct sockaddr_flex *)msg->msg_name;
  struct sk_buff *skb;
  struct uflexhdr *fhdr;
  unsigned char ptype;

  skb = __skb_recv_datagram(sk, flags, flex_skb_destructor, &peeked, &off, &err);

  if (!skb) goto out;

  if (skb->len == 0)
    goto out;

  fhdr = (struct uflexhdr *)(skb->head + skb->network_header);
  FLEX_LOG1x("Hash Type", fhdr->common.hash_type);
  FLEX_LOG1d("Hop Limit", fhdr->common.hop_limit);
  FLEX_LOG1d("Header Length", ntohs(fhdr->common.header_len));
  FLEX_LOG1x("Checksum", ntohs(fhdr->common.check));
  FLEX_LOG1x("Packet ID", ntohs(fhdr->common.frag_off));
  FLEX_LOG1x("Packet Type", fhdr->common.packet_type);
  FLEX_LOG1d("Packet Length", ntohs(fhdr->packet_len));

  ptype = fhdr->common.packet_type;       // Packet Type
  hlen = ntohs(fhdr->common.header_len);  // Header Length
  plen = ntohs(fhdr->packet_len);         // Packet Length (Header Length + Data)
  bytes = 0;

  switch(ptype)
  {
    case FLEX_INTEREST:
      bytes = copy_to_iter(fhdr + DFLEX_ID_IDX, FLEX_ID_LENGTH, &msg->msg_iter);
      FLEX_LOG1d("Bytes to application", bytes);
      break;
    case FLEX_DATA:
      bytes = copy_to_iter(fhdr + hlen, plen - hlen, &msg->msg_iter);
      FLEX_LOG1d("Bytes to application", bytes);
      break;
    case FLEX_DATA_ACK:
      break;
    default:
      err = -NO_SUPPORT;
      goto out;
  }

	return bytes;

out:
  return err;
}

/**
 * @brief Destructor for Socket Buffer from the Socket
 * @param sk Socket Structure
 * @param skb Socket Buffer
 */
void flex_skb_destructor(struct sock *sk, struct sk_buff *skb)
{
  FLEX_LOG("Destructor");
}

/**
 * @brief Release the socket
 * @param sock The Socket to be released
 * @return Error code
 */
int flex_unreliable_release(struct socket *sock)
{
  struct sock *sk;
  struct flex_sock *flex;
  struct flexid_entity *entity;
  struct id_table *table;
  struct id_hslot *hslot;
  unsigned int slot;

	FLEX_LOG("Release the socket internally");

  sk = sock->sk;

  if (!sk)
    return 0;

  flex = flex_sk(sk);
  table = &id_table;

  if (flex->message == FLEX_INTEREST)
  {
    FLEX_LOG("This is INTEREST socket");
    slot = hash_fn(flex->dst, table->mask);
  }
  else if (flex->message == FLEX_DATA)
  {
    FLEX_LOG("This is DATA socket");
    slot = hash_fn(flex->src, table->mask);
  }
  else
  {
    goto no_slot;
  }
  hslot = &table->hash[slot];
  FLEX_LOG("Find the hash slot");

  spin_lock_bh(&hslot->lock);
  hlist_for_each_entry_rcu(entity, &hslot->head, flex_node)
  {
    if (entity->sk == sk)
    {
      hlist_del(&entity->flex_node);
      hslot->count--;
    }
  }
  spin_unlock_bh(&hslot->lock);

no_slot:
  FLEX_LOG("Come to no slot");
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
