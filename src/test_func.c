#include <stdio.h>
#include <stdlib.h>

#include <flex/flex_const.h>
#include <flex/flex_id.h>
#include <flex/flex_query.h>
#include <flex/flex_request.h>
#include <flex/flex_log.h>
#include <flex/flex_types.h>
#include <flex/flex_addr.h>
#include <flex/flex_err.h>

int test_query(flexid_t **id)
{
  int err;
  char *buf = "Hello, World!";
  if ((err = init_flexid(id, buf, FLEX_TYPE_CONTENT)) < 0)
    goto out;

  return SUCCESS;

out:
  return err;
}

int test_request(flexid_t *id, response_t **ret)
{
  int i, err;
  (*ret) = (response_t *)malloc(sizeof(response_t));

  if (!(*ret))
  {
    APP_LOG("Error in malloc");
    err = -ERROR_MALLOC;
    goto out;
  }

  (*ret)->error = TRUE;
  (*ret)->addr_type = AT_MAC;
  (*ret)->addr_len = AT_MAC_LEN;
  
  (*ret)->next_hop[0] = 0x00;
  (*ret)->next_hop[1] = 0x0c;
  (*ret)->next_hop[2] = 0x29;
  (*ret)->next_hop[3] = 0xcb;
  (*ret)->next_hop[4] = 0x8d;
  (*ret)->next_hop[5] = 0x25;

  return SUCCESS;

out:
  return err;
}
