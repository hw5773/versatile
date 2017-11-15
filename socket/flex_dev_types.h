#ifndef __FLEX_DEV_TYPES__
#define __FLEX_DEV_TYPES__

#include <linux/if_arp.h>

/* Device types */
#define MAX_DEV_TYPES 4

short dev_type[MAX_DEV_TYPES] = 
{ 
  ARPHRD_ETHER, 
  ARPHRD_IEEE802, 
  ARPHRD_IEEE80211, 
  ARPHRD_IEEE802154,
};

#endif /* __FLEX_DEV_TYPES__ */
