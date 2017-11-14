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

	printk(KERN_INFO "[Flex] %s: Enter Flex Receive Routine\n", __func__);

	// Parse *data to *flex
	printk(KERN_INFO "[Flex] %s: version = 0x%x\n", __func__, flex->version);
	printk(KERN_INFO "[Flex] %s: version = 0x%x\n", __func__, flex->packet_type);
	printk(KERN_INFO "[Flex] %s: version = 0x%x\n", __func__, flex->hash_type);
	
	return 0;
}
