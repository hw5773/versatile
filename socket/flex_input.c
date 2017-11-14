#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <net/sock.h>
#include <linux/slab.h>

#include "flex_sock.h"

int flex_rcv(struct sk_buff *skb, struct net_device *dev, 
			struct packet_type *ptype, struct net_device *orig_dev)
{
  struct flexhdr *flex;
  flex = (struct flexhdr *)skb->data;

	FLEX_LOG("Enter Flex Receive Routine");

	// Parse *data to *flex
	FLEX_LOG1x("Version", flex->version);
	FLEX_LOG1x("Packet Type", flex->packet_type);
	FLEX_LOG1x("Hash Type", flex->hash_type);
	
	return 0;
}
