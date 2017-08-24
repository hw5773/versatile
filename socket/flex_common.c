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

static int flex_release(struct socket *)
{
	FLEX_LOG("Release the socket");
}

static int flex_bind(struct socket *, struct sockaddr *, int)
{
	FLEX_LOG("Bind for Flex");
}

static int flex_getname(struct socket *, struct sockaddr *, int *, int)
{
	FLEX_LOG("Get the name");
}

static int flex_ioctl(struct socket *, unsigned int, unsigned long)
{
	FLEX_LOG("I/O Control for Flex");
}

static int flex_shutdown(struct socket *, int)
{
	FLEX_LOG("Shutdown");
}

static int flex_setsockopt(struct socket *, int level, int optname,
        char __user *optval, unsigned int optlen)
{
	FLEX_LOG("Set the socket options");
}
static int flex_getsockopt(struct socket *, int level, int optname,
        char __user *optval, int __user *optlen)
{
	FLEX_LOG("Get the socket options");
}
