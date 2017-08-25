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

int flex_reliable_connect(struct socket *sock, struct sockaddr *addr, int addr_len, int flags)
{
	FLEX_LOG("Reliable Connect");
	return -1;
}

int flex_accept(struct socket *sock, struct socket *newsock, int flags)
{
	FLEX_LOG("Accept");
	return -1;
}

unsigned int flex_reliable_poll(struct file *file, struct socket *sock, poll_table *wait)
{
	FLEX_LOG("Reliable Polling");
	return -1;
}

int flex_listen(struct socket *sock, int backlog)
{
	FLEX_LOG("Listen");
	return -1;
}

int flex_reliable_sendmsg(struct socket *sock, struct msghdr *msg, size_t size)
{
	FLEX_LOG("Send the reliable message");
	return -1;
}

int flex_reliable_recvmsg(struct socket *sock, struct msghdr *msg, size_t size, int flags)
{
	FLEX_LOG("Receive the reliable message");
	return -1;
}
