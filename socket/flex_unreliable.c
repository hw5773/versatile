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
  }
  else
    FLEX_LOG1s("Device Name", netdev_name(dev));

  skb = alloc_skb(sizeof(struct uflexhdr) + LL_RESERVED_SPACE(dev), GFP_ATOMIC);

  if (skb == NULL)
  {
    FLEX_LOG("Allocate Socket Buffer Failed");
    goto out;
  }

  FLEX_LOG("Initialize the Flex Header");

  skb_reserve(skb, LL_RESERVED_SPACE(dev));
  skb->dev = dev;
  skb->protocol = htons(ETH_P_FLEX);

  flexh = (struct uflexhdr *)skb_put(skb, sizeof(struct uflexhdr));
  flexh->common.version = FLEX_1_0;

  FLEX_LOG("Find the message type");
  
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
  flexh->common.frag_off = htons(FLEX_PTC | FLEX_DF | 0x365);
  memset(flexh->sflex_id, '1', FLEX_ID_LENGTH);
  memcpy(flexh->dflex_id, flex->dst, flex->dst.length);
  flexh->packet_len = htons(UNRELIABLE_HEADER_LEN);

	return SUCCESS;

out:
  return err;
}

int flex_unreliable_recvmsg(struct socket *sock, struct msghdr *msg, size_t size, int flags)
{
	FLEX_LOG("Receive the unreliable message");
	return -1;
}
