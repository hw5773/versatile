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
  int i, err;
  (*id) = (flexid_t *)malloc(sizeof(flexid_t));

  set_cache_bit((*id), TRUE);
  set_segment_bit((*id), FALSE);
  set_collision_avoidance_bit((*id), FALSE);
  
  for (i=0; i<10; i++)
    (*id)->identity[i] = 0x41;

  for (i=10; i<20; i++)
    (*id)->identity[i] = 0x42;

  (*id)->length = FLEX_ID_LENGTH;

  APP_LOG1d("First Bit", (*id));

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
  
  (*ret)->next_hop[0] = FIRST;
  (*ret)->next_hop[1] = SECOND;
  (*ret)->next_hop[2] = THIRD;
  (*ret)->next_hop[3] = FOURTH;
  (*ret)->next_hop[4] = FIFTH;
  (*ret)->next_hop[5] = SIXTH;

  return SUCCESS;

out:
  return err;
}
