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

static const struct proto_ops flex_reliable_ops = {
	.family		=	PF_FLEX,
	.owner		=	THIS_MODULE,
	.release	=	flex_release,
	.bind		=	flex_bind,
	.connect	=	flex_reliable_connect,
	.socketpair	=	sock_no_socketpair,
	.accept		=	flex_accept,
	.getname	=	flex_getname,
	.poll		=	flex_reliable_poll,
	.ioctl		=	flex_ioctl,
	.listen		=	flex_listen,
	.shutdown	=	flex_shutdown,
	.setsockopt	=	flex_setsockopt,
	.getsockopt	=	flex_getsockopt,
	.sendmsg	=	flex_reliable_sendmsg,
	.recvmsg	=	flex_reliable_recvmsg,
	.mmap		=	sock_no_mmap,
	.sendpage	=	sock_no_sendpage,
};

static const struct proto_ops flex_unreliable_ops = {
	.family		=	PF_FLEX,
	.owner		=	THIS_MODULE,
	.release	=	flex_release,
	.bind		=	flex_bind,
	.connect	=	flex_unreliable_connect,
	.socketpair	=	sock_no_socketpair,
	.accept		=	sock_no_accept,
	.getname	=	sock_no_getname,
	.poll		=	datagram_poll,
	.ioctl		=	sock_no_ioctl,
	.listen		=	sock_no_listen,
	.shutdown	=	flex_shutdown,
	.setsockopt	=	sock_no_setsockopt,
	.getsockopt	=	sock_no_getsockopt,
	.sendmsg	=	flex_unreliable_sendmsg,
	.recvmsg	=	flex_unreliable_recvmsg,
	.mmap		=	sock_no_mmap,
	.sendpage	=	sock_no_sendpage,
};

static struct sock *flex_alloc_socket(struct net *net)
{
	struct flex_sock *flex;
	struct sock *sk = sk_alloc(net, AF_FLEX, GFP_ATOMIC, &flex_proto, 0);

	FLEX_LOG("Enter the flex socket allocator");

	if (!sk)
		goto out;

	sock_init_data(NULL, sk);

	flex = flex_sk(sk);

	/* TODO: Add something. Need to understand the socket creation process. */

out:
	return sk;
}

static int flex_create(struct net *net, struct socket *sock, int protocol, int kern)
{
	struct sock *sk;
	struct flex_sock *flex;
	int rc;

	FLEX_LOG("Enter the flex protocol creator");

	if (protocol && protocol != PF_FLEX)
		return -EPROTONOSUPPORT;

	FLEX_LOG("Confirm the protocol of the socket is PF_FLEX");

	sock->state = SS_UNCONNECTED;

	FLEX_LOG("Set the state of the socket as SS_UNCONNECTED");

	rc = -ENOBUFS;
	if ((sk = flex_alloc_socket(net)) == NULL)
		goto out;

	FLEX_LOG("Allocate the flex socket");

	flex = flex_sk(sk);

	sock_init_data(sock, sk);

	FLEX_LOG("Initialize the socket data");

	switch (sock->type) {
	case SOCK_STREAM:
		sock->ops = &flex_reliable_ops;
		FLEX_LOG("Set the socket operations as reliable");
		break;
	case SOCK_DGRAM:
		sock->ops = &flex_unreliable_ops;
		FLEX_LOG("Set the socket operations as unreliable");
		break;
	default:
		return -ESOCKTNOSUPPORT;
	}

	FLEX_LOG("Socket create success!");

	rc = 0;
out:
	return rc;
}

static const struct net_proto_family flex_family_ops = {
	.family 	=	PF_FLEX,
	.create		=	flex_create,
	.owner		=	THIS_MODULE,
};

static void __exit af_flex_exit(void)
{
	FLEX_LOG("Enter the flex protocol destructor");
	dev_remove_pack(&flex_packet_type);
	FLEX_LOG("Remove the flex packet success!");
	sock_unregister(AF_FLEX);
	FLEX_LOG("Unregister the flex socket");
}

static int __init af_flex_init(void)
{
	int rc;
	FLEX_LOG("Enter the flex protocol initializer");
	rc = proto_register(&flex_proto, 1);

	if (rc != 0)
	{
		FLEX_LOG("Cannot create flex_sock SLAB cache!");
		goto out;
	}

	rc = sock_register(&flex_family_ops);

	if (rc != 0)
	{
		FLEX_LOG("Cannot register flex socket!");
		goto out_proto;
	}

	dev_add_pack(&flex_packet_type);

	FLEX_LOG("The Protocal and Socket for Flex ID are registered");

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
