#ifndef __FLEX_SOCKET__
#define __FLEX_SOCKET__

#include <linux/socket.h>
#include <flex/flex_id.h>

/* Flex socket address struct */
struct sockaddr_flex 
{
  __kernel_sa_family_t  sin_family;
  struct flexid         id;
  int                   message;
};

/* Change to the Flex socket address struct */
static inline struct sockaddr_flex *target_info(struct sockaddr *addr)
{
  return (struct sockaddr_flex *)addr;
}

#endif /* __FLEX_SOCKET__ */
