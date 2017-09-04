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
#include <linux/netdevice.h>

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

int flex_unreliable_sendmsg_test(struct socket *sock, struct msghdr *msg, size_t size)
{
	struct net_device *dev;
	struct sk_buff *skb;
	struct flexhdr *flex;

	FLEX_LOG("Send the unreliable message");

	dev = dev_get_by_name(&init_net, "enp1s0");
	skb = alloc_skb(sizeof(struct flexhdr) + LL_RESERVED_SPACE(dev), GFP_ATOMIC);

	if (skb == NULL)
	{
		FLEX_LOG("Allocate Socket Buffer Failed");
		goto out;
	}

	skb_reserve(skb, LL_RESERVED_SPACE(dev));
	skb->dev = dev;
	skb->protocol = htons(ETH_P_FLEX);

	flex = (struct flexhdr *)skb_put(skb, sizeof(struct flexhdr));
	flex->version = FLEX_1_0;
	flex->packet_type = FLEX_JOIN;
	flex->hash_type = SHA1;
	flex->hop_limit = DEFAULT_HOP_LIMIT;
	flex->header_len = htons(DEFAULT_HEADER_LEN);
	flex->check = htons(0x1234);
	flex->packet_id = htons(0x7777);
	flex->frag_off = htons(0x8000 | 0x2000 | 0x365);
	memset(flex->sflex_id, '1', FLEX_ID_LENGTH);
	memset(flex->dflex_id, '7', FLEX_ID_LENGTH);
	flex->packet_len = htons(DEFAULT_HEADER_LEN);
	flex->seq = htonl(0x12345678);
	flex->ack = htonl(0x98765432);

	if (dev_hard_header(skb, dev, ETH_P_FLEX, dev->broadcast, dev->dev_addr, skb->len) < 0)
	{
		FLEX_LOG("Make Header Frame Failed");
		goto out;
	}

	FLEX_LOG("Make Header Frame Success");
	dev_queue_xmit(skb);
	FLEX_LOG("Send the Frame");
	return -1;

out:
	return -1;
}

int flex_unreliable_recvmsg(struct socket *sock, struct msghdr *msg, size_t size, int flags)
{
	FLEX_LOG("Receive the unreliable message");
	return -1;
}
