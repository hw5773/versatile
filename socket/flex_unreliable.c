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

int flex_unreliable_connect(struct socket *sock, struct sockaddr *addr, int addr_len, int flags)
{
	FLEX_LOG("Unreliable Connect");
	return -1;
}

unsigned int flex_unreliable_poll(struct file *file, struct socket *sock, poll_table *wait)
{
	FLEX_LOG("Unreliable Polling");
	return -1;
}

int flex_unreliable_sendmsg(struct socket *sock, struct msghdr *msg, size_t size)
{
	FLEX_LOG("Send the unreliable message");
	return -1;
}

int flex_unreliable_recvmsg(struct socket *sock, struct msghdr *msg, size_t size, int flags)
{
	FLEX_LOG("Receive the unreliable message");
	return -1;
}
