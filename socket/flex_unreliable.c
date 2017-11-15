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
#include "flex_dev_types.h"

int flex_unreliable_connect(struct socket *sock, struct sockaddr *taddr, int addr_len, int flags)
{
  struct sock *sk;
  struct flex_sock *flex;
  struct sockaddr_flex *tinfo;

	FLEX_LOG("Unreliable Connect");

  sk = sock->sk;
  flex = flex_sk(sk);
  tinfo = target_info(taddr);
  flex->dst = tinfo->id;
  flex->message = tinfo->message;

	return SUCCESS;
}

unsigned int flex_unreliable_poll(struct file *file, struct socket *sock, poll_table *wait)
{
	FLEX_LOG("Unreliable Polling");
	return -1;
}

int flex_unreliable_sendmsg(struct socket *sock, struct msghdr *msg, size_t size)
{
  int i;
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

  // TODO: Error Type 
  if (!dev)
  {
    FLEX_LOG("Error: No device to send");
    return -NODEV;
  }
  else
    FLEX_LOG1s("Device Name", netdev_name(dev));

  FLEX_LOG("Find the message type");
  
//  switch (flex->message)
//  {
//    case FLEX_INTEREST

	return -1;
}

int flex_unreliable_sendmsg_test(struct socket *sock, struct msghdr *msg, size_t size)
{
	struct net_device *dev;
  //struct sock *sk = sock->sk;
  //struct flex_sock *flex = flex_sk(sk);
  DECLARE_SOCKADDR(struct sockaddr_flex *, usflex, msg->msg_name);
	struct sk_buff *skb;
	struct uflexhdr *flexh;
  unsigned char *content;

  FLEX_LOG("Confirm the content of msghdr");
  printk(KERN_INFO "[Flex] %s: msg_namelen: %d\n", __func__, msg->msg_namelen);
  printk(KERN_INFO "[Flex] %s: size: %lu\n", __func__, size);
  content = (unsigned char *)kmalloc(10, GFP_ATOMIC);
  memcpy_from_msg(content, msg, size);
  FLEX_LOG(content);

	FLEX_LOG("Send the unreliable message");

	dev = dev_get_by_name(&init_net, "ens33");
	skb = alloc_skb(sizeof(struct uflexhdr) + LL_RESERVED_SPACE(dev), GFP_ATOMIC);

	if (skb == NULL)
	{
		FLEX_LOG("Allocate Socket Buffer Failed");
		goto out;
	}

	skb_reserve(skb, LL_RESERVED_SPACE(dev));
	skb->dev = dev;
	skb->protocol = htons(ETH_P_FLEX);

	flexh = (struct uflexhdr *)skb_put(skb, sizeof(struct uflexhdr));
	flexh->common.version = FLEX_1_0;
	flexh->common.packet_type = FLEX_JOIN;
	flexh->common.hash_type = SHA1;
	flexh->common.hop_limit = DEFAULT_HOP_LIMIT;
	flexh->common.header_len = htons(UNRELIABLE_HEADER_LEN);
	flexh->common.check = htons(0x1234);
	flexh->common.packet_id = htons(0x7777);
	flexh->common.frag_off = htons(FLEX_PTC | FLEX_MF | 0x365);
	memset(flexh->sflex_id, '1', FLEX_ID_LENGTH);
	memset(flexh->dflex_id, '7', FLEX_ID_LENGTH);
	flexh->packet_len = htons(UNRELIABLE_HEADER_LEN);

	if (dev_hard_header(skb, dev, ETH_P_FLEX, dev->broadcast, dev->dev_addr, skb->len) < 0)
	{
		FLEX_LOG("Make Header Frame Failed");
		goto out;
	}

	FLEX_LOG("Make Header Frame Success");
	dev_queue_xmit(skb);
	FLEX_LOG("Send the Frame");
  kfree(content);
	return -1;

out:
	return -1;
}

int flex_unreliable_recvmsg(struct socket *sock, struct msghdr *msg, size_t size, int flags)
{
	FLEX_LOG("Receive the unreliable message");
	return -1;
}
