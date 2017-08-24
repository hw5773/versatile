#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/dcache.h>
#include <linux/namei.h>
#include <linux/socket.h>
#include <linux/un.h>
#include <linux/fcntl.h>
#include <linux/termios.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <net/net_namespace.h>
#include <net/sock.h>
#include <net/tcp_states.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <net/scm.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/rtnetlink.h>
#include <linux/mount.h>
#include <net/checksum.h>
#include <linux/security.h>

#include "flex.h"

//#define PF_FLEX PF_PACKET
//#define AF_FLEX AF_PACKET

static int flex_create(struct net *net, struct socket *sock, int protocol, int kern)
{
	printk(KERN_INFO "[Flex] %s: Enter the flex protocol creator\n", __func__);
	return 0;
}

static struct packet_type flex_packet_type __read_mostly = 
{
	.type		=	cpu_to_be16(ETH_P_FLEX),
	.func		= 	flex_rcv,
};

static struct proto flex_proto = {
	.name		=	"FLEX",
	.owner		=	THIS_MODULE,
	.obj_size	=	sizeof(struct flex_sock),
};

static const struct net_proto_family flex_family_ops = {
	.family 	=	PF_FLEX,
	.create		=	flex_create,
	.owner		=	THIS_MODULE,
};

static void __exit af_flex_exit(void)
{
	printk(KERN_INFO "[Flex] %s: Enter the flex protocol destructor\n", __func__);
	dev_remove_pack(&flex_packet_type);
	printk(KERN_INFO "[Flex] %s: Remove the flex packet success!\n", __func__);
	proto_unregister(&flex_proto);
	printk(KERN_INFO "[Flex] %s: Unregister the flex protocol\n", __func__);
	sock_unregister(AF_FLEX);
	printk(KERN_INFO "[Flex] %s: Unregister the flex socket\n", __func__);
}

static int __init af_flex_init(void)
{
	int rc;
	printk(KERN_INFO "[Flex] %s: Enter the flex protocol initializer\n", __func__);
	rc = proto_register(&flex_proto, 1);

	if (rc != 0)
	{
		printk(KERN_CRIT "[Flex] %s: Cannot create flex_sock SLAB cache!\n", __func__);
		goto out;
	}

	rc = sock_register(&flex_family_ops);

	if (rc != 0)
	{
		printk(KERN_CRIT "[Flex] %s: Cannot register flex socket!\n", __func__);
		goto out_proto;
	}

	dev_add_pack(&flex_packet_type);

	printk(KERN_INFO "[Flex] The Protocal and Socket for Flex ID are registered\n");

out_proto:
	proto_unregister(&flex_proto);
out:
	return rc;
}

module_init(af_flex_init);
module_exit(af_flex_exit);
MODULE_LICENSE("GPL");
MODULE_ALIAS_NETPROTO(PF_FLEX);
MODULE_AUTHOR(PROTOCOL_AUTHOR);
MODULE_DESCRIPTION(PROTOCOL_DESC);
