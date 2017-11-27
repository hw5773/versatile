#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <net/sock.h>

#include "flex_sock.h"
#include "flex_idtable.h"

/**
 * @brief Handler for the Flex Network Packet in the network layer
 * @param skb Socket Buffer (Flex Network Packet)
 * @param dev Hardware Device for send/receive
 * @param ptype Not used
 * @param orig_dev Not used
 * @return Error code
 */
int flex_rcv(struct sk_buff *skb, struct net_device *dev, 
			struct packet_type *ptype, struct net_device *orig_dev)
{
  int ret;
  flexhdr_t *flex;
  struct sock *sk;
  flexid_t *fid;

	FLEX_LOG("Enter Flex Receive Routine");

  flex = (flexhdr_t *)skb->data;
  fid = (flexid_t *)kmalloc(sizeof(flexid_t), GFP_ATOMIC);

  if (GET_FLEX_PTC(flex->frag_off))
    FLEX_LOG("Received Packet uses Reliable Communication");
  else
  {
    uflexhdr_t *uflex;
    FLEX_LOG("Received Packet uses Unreliable Communication");
    uflex = (uflexhdr_t *)flex;
    memcpy(fid, uflex->dflex_id, FLEX_ID_LENGTH);
    fid->length = FLEX_ID_LENGTH;
  	sk = get_sock_by_id(fid, &id_table);

    if (!sk)
    {
      FLEX_LOG("Cannot find the appropriate socket");
      goto out;
    }

    FLEX_LOG("Find the appropriate socket");
    ret = sock_queue_rcv_skb(sk, skb);
    FLEX_LOG1d("Invoke sock_queue_rcv_skb complete", ret);
  }

  kfree(fid);
	return SUCCESS;

out:
  return FAILURE;
}
