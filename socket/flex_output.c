#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <net/sock.h>

int flex_output(struct sk_buff *skb)
{
	printk(KERN_INFO "[Flex] %s: Enter Flex Transmit Routine\n", __func__);
	return 0;
}
