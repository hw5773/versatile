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
#include <flex/flex.h>
#include <flex/flex_repo.h>

#include <sys/socket.h>
#include <sys/types.h>

unsigned long get_current_microseconds()
{
  struct timeval curr;
  gettimeofday(&curr, NULL);

  return curr.tv_sec * 1000000 + curr.tv_usec;
}

int test_pub()
{
  int i, err;
  struct sockaddr_flex insert_id;
  flexid_t *tid;

  err = -ERROR_MALLOC;
  if (!(tid = (flexid_t *)malloc(sizeof(flexid_t)))) goto out;

  set_cache_bit(tid, TRUE);
  set_segment_bit(tid, FALSE);
  set_collision_avoidance_bit(tid, FALSE);

  for (i=0; i<10; i++)
    tid->identity[i] = 0x41;

  for (i=10; i<20; i++)
    tid->identity[i] = 0x42;

  tid->length = FLEX_ID_LENGTH;

  insert_id.sid = *tid;
  insert_id.message = FLEX_DATA;

  err = -ERROR_BIND;
  if ((bind(urepo_sock, (struct sockaddr *)&insert_id, sizeof(insert_id))) < 0) goto out;

  APP_LOG1d("ID Length", tid->length);

  if ((err = add_id_name_map(tid, TEST_FILE_PATH)) < 0) goto out;

  return SUCCESS;

out:
  return err;
}

int test_sid(flexid_t **id)
{
  int i, err;
  (*id) = (flexid_t *)malloc(sizeof(flexid_t));

  set_cache_bit((*id), FALSE);
  set_segment_bit((*id), FALSE);
  set_collision_avoidance_bit((*id), TRUE);
  
  for (i=0; i<10; i++)
    (*id)->identity[i] = 0x46;

  for (i=10; i<20; i++)
    (*id)->identity[i] = 0x47;

  (*id)->length = FLEX_ID_LENGTH;

  return SUCCESS;

out:
  return err;
}

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
