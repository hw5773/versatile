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

static int flex_reliable_connect(struct socket *, struct sockaddr *, int addr_len, int flags)
{
	FLEX_LOG("Reliable Connect");
}

static int flex_accept(struct socket *, struct socket *, int)
{
	FLEX_LOG("Accept");
}

static unsigned int flex_reliable_poll(struct file *, struct socket *, poll_table *)
{
	FLEX_LOG("Reliable Polling");
}

static int flex_listen(struct socket *, int backlog)
{
	FLEX_LOG("Listen");
}

static int flex_reliable_sendmsg(struct kiocb *, struct socket *, struct msghdr *, size_t)
{
	FLEX_LOG("Send the reliable message");
}

static int flex_reliable_recvmsg(struct kiocb *, struct socket *, struct msghdr *, size_t, int)
{
	FLEX_LOG("Receive the reliable message");
}
