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

#include "flex.h"

int flex_release(struct socket *sock)
{
	FLEX_LOG("Release the socket");
	return 0;
}

int flex_bind(struct socket *sock, struct sockaddr *addr, int addr_len)
{
	FLEX_LOG("Bind for Flex");
	return -1;
}

int flex_getname(struct socket *sock, struct sockaddr *addr, int *addr_len, int peer)
{
	FLEX_LOG("Get the name");
	return -1;
}

int flex_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	FLEX_LOG("I/O Control for Flex");
	return -1;
}

int flex_shutdown(struct socket *sock, int how)
{
	FLEX_LOG("Shutdown");
	return -1;
}

int flex_setsockopt(struct socket *sock, int level, int optname,
        char __user *optval, unsigned int optlen)
{
	FLEX_LOG("Set the socket options");
	return -1;
}
int flex_getsockopt(struct socket *sock, int level, int optname,
        char __user *optval, int __user *optlen)
{
	FLEX_LOG("Get the socket options");
	return -1;
}
