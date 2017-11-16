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
  int i, err;
  struct net_device *dev;
  uflexhdr_t *rcv, *flex;
  unsigned char target_addr[6];
  FLEX_LOG("Enter Test Output Routine");

  for (i=0; i<MAX_DEV_TYPES; i++)
  {
    dev = dev_getfirstbyhwtype(&init_net, dev_type[i]);

    if (dev)
      break;
  }

  if (!dev)
  {
    FLEX_LOG("Error: No device to send");
    err = -NO_DEV;
    goto out;
  }
  else
    FLEX_LOG1s("Device name", netdev_name(dev));

  skb = alloc_skb(sizeof(uflexhdr_t) + LL_RESERVED_SPACE(dev), GFP_ATOMIC);

  if (!skb)
  {
    FLEX_LOG("Allocate Socket Buffer Failed");
    goto out;

  target_addr[0] = 0x00;
  target_addr[1] = 0x0c;
  target_addr[2] = 0x29;
  target_addr[3] = 0x32;
  target_addr[4] = 0x2a;
  target_addr[5] = 0xfc;

  rcv = (uflexhdr_t *)skb->data;

  return SUCCESS;
}
