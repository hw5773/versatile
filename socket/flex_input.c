#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <net/sock.h>

#include "flex_sock.h"

int flex_rcv(struct sk_buff *skb, struct net_device *dev, 
			struct packet_type *ptype, struct net_device *orig_dev)
{
  struct flexhdr *flex;
  flex = (struct flexhdr *)skb->data;

	FLEX_LOG("Enter Flex Receive Routine");

  if (GET_FLEX_PTC(flex->frag_off))
    FLEX_LOG("Received Packet uses Reliable Communication");
  else
  {
    FLEX_LOG("Received Packet uses Unreliable Communication");
  	test_output(skb);
  }

	return SUCCESS;
}
