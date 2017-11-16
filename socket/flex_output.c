#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <net/sock.h>

#include "flex_sock.h"

int flex_output(struct sk_buff *skb)
{
	FLEX_LOG("Enter Flex Transmit Routine");
	return SUCCESS;
}

int test_output(struct sk_buff *skb)
{
  int err;
  struct flexhdr *flex;
  FLEX_LOG("Enter Test Output Routine");

  return SUCCESS;
}
