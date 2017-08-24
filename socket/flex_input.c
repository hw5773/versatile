#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <net/sock.h>

int flex_rcv(struct sk_buff *skb, struct net_device *dev, 
			struct packet_type *ptype, struct net_device *orig_dev)
{
	printk(KERN_INFO "[Flex] %s: Enter Flex Receive Routine\n", __func__);
	return 0;
}
