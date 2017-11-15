#ifndef __FLEX_REQUEST__
#define __FLEX_REQUEST__

#include <flex/flex_addr.h>
#include <flex/flex_id.h>

struct response
{
  short error;
  short addr_type;
  unsigned char addr_len;
  unsigned char next_hop[MAX_ADDRESS_LENGTH];
};

struct response *request(struct flexid *);

int init_response(struct response **);
int free_response(struct response *);

#endif /* __FLEX_REQUEST__ */
