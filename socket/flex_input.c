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
	char *p, *data;
	struct flexhdr *flex;
	int index = 0;
	flex = (struct flexhdr *)kmalloc(sizeof(struct flexhdr), GFP_KERNEL);
	data = (char *)kmalloc(skb->len, GFP_KERNEL);

	if(!flex || !data) {
		printk(KERN_INFO "[Flex] %s: kmalloc failed\n", __func__);
		return -1;	
	}

	printk(KERN_INFO "[Flex] %s: Enter Flex Receive Routine\n", __func__);

	for (p = skb->data; p < (skb->tail); p++) {
		data[index++] = *p;
	}

	// Parse *data to *flex
	memcpy(flex, data, skb->len);
	printk(KERN_INFO "[Flex] %s: version = 0x%x\n", __func__, flex->version);
	printk(KERN_INFO "[Flex] %s: version = 0x%x\n", __func__, flex->packet_type);
	printk(KERN_INFO "[Flex] %s: version = 0x%x\n", __func__, flex->hash_type);
	printk(KERN_INFO "[Flex] %s: sflex_id = %s\n", __func__, flex->sflex_id);
	printk(KERN_INFO "[Flex] %s: dflex_id = %s\n", __func__, flex->dflex_id);
	
	kfree(flex);
	kfree(data);
		
	return 0;
}
