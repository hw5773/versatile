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
  unsigned char a = 0x80;
  flexid_t *tid;

  err = -ERROR_MALLOC;
  if (!(tid = (flexid_t *)malloc(sizeof(flexid_t)))) goto out;

  APP_LOG("Initialize the Data Repository");

  set_cache_bit(tid, TRUE);
  set_segment_bit(tid, FALSE);
  set_collision_avoidance_bit(tid, FALSE);
/*
  for (i=0; i<10; i++)
    tid->identity[i] = 0x41;

  for (i=10; i<20; i++)
    tid->identity[i] = 0x42;
*/
  tid->identity[0] = 0x40;
  tid->identity[1] = 0xf0;
  tid->identity[2] = 0x67;
  tid->identity[3] = 0xc1;
  tid->identity[4] = 0xcd;
  tid->identity[5] = 0x83;
  tid->identity[6] = 0x96;
  tid->identity[7] = 0x2a;
  tid->identity[8] = 0xbf;
  tid->identity[9] = 0x35;
  tid->identity[10] = 0x99;
  tid->identity[11] = 0xde;
  tid->identity[12] = 0x79;
  tid->identity[13] = 0xc1;
  tid->identity[14] = 0xab;
  tid->identity[15] = 0xb7;
  tid->identity[16] = 0x7b;
  tid->identity[17] = 0x2f;
  tid->identity[18] = 0x02;
  tid->identity[19] = 0x6d;

  tid->length = FLEX_ID_LENGTH;

  APP_LOG("Set the Test Flex ID");
  APP_LOGid("Content ID", tid, tid->length);

  insert_id.sid = *tid;
  insert_id.message = FLEX_DATA;

  //APP_LOG("Set the ID and Message Type");

  err = -ERROR_BIND;
  if ((bind(urepo_sock, (struct sockaddr *)&insert_id, sizeof(insert_id))) < 0) goto out;

  if ((err = add_id_name_map(tid, TEST_FILE_PATH)) < 0) goto out;

  return SUCCESS;

out:
  return err;
}

int test_sid(flexid_t **id, int num)
{
  int i, err;
  unsigned char a = 0x36;
  (*id) = (flexid_t *)malloc(sizeof(flexid_t));
/*
  set_cache_bit((*id), FALSE);
  set_segment_bit((*id), FALSE);
  set_collision_avoidance_bit((*id), TRUE);
*/
  memcpy((*id), &a, 1);
  err = -1;
  (*id)->identity[0] = rand() % 256;
  (*id)->identity[1] = rand() % 256;
  (*id)->identity[2] = 0x55;
  (*id)->identity[3] = rand() % 256;
  (*id)->identity[4] = 0xcc;
  (*id)->identity[5] = 0x33;
  (*id)->identity[6] = rand() % 256;
  (*id)->identity[7] = 0x2a;
  (*id)->identity[8] = 0xbf;
  (*id)->identity[9] = rand() % 256;
  (*id)->identity[10] = rand() % 256;
  (*id)->identity[11] = rand() % 256;
  (*id)->identity[12] = rand() % 256;
  (*id)->identity[13] = rand() % 256;
  (*id)->identity[14] = 0xa;
  (*id)->identity[15] = 0xb7;
  (*id)->identity[16] = 0xb;
  (*id)->identity[17] = rand() % 256;
  (*id)->identity[18] = rand() % 256;
  (*id)->identity[19] = 0x6d;

/*
  for (i=0; i<10; i++)
    (*id)->identity[i] = 0x46;

  for (i=10; i<20; i++)
    (*id)->identity[i] = 0x47;
*/

  (*id)->length = FLEX_ID_LENGTH;

  return SUCCESS;

out:
  return err;
}

int test_query(flexid_t **id)
{
  int i, err;
  unsigned char a = 0x80;
  (*id) = (flexid_t *)malloc(sizeof(flexid_t));
/*
  set_cache_bit((*id), TRUE);
  set_segment_bit((*id), FALSE);
  set_collision_avoidance_bit((*id), FALSE);
  
  for (i=0; i<10; i++)
    (*id)->identity[i] = 0x41;

  for (i=10; i<20; i++)
    (*id)->identity[i] = 0x42;
*/
  memcpy((*id), &a, 1);
  (*id)->identity[0] = 0x40;
  (*id)->identity[1] = 0xf0;
  (*id)->identity[2] = 0x67;
  (*id)->identity[3] = 0xc1;
  (*id)->identity[4] = 0xcd;
  (*id)->identity[5] = 0x83;
  (*id)->identity[6] = 0x96;
  (*id)->identity[7] = 0x2a;
  (*id)->identity[8] = 0xbf;
  (*id)->identity[9] = 0x35;
  (*id)->identity[10] = 0x99;
  (*id)->identity[11] = 0xde;
  (*id)->identity[12] = 0x79;
  (*id)->identity[13] = 0xc1;
  (*id)->identity[14] = 0xab;
  (*id)->identity[15] = 0xb7;
  (*id)->identity[16] = 0x7b;
  (*id)->identity[17] = 0x2f;
  (*id)->identity[18] = 0x02;
  (*id)->identity[19] = 0x6d;

  (*id)->length = FLEX_ID_LENGTH;

  return SUCCESS;

out:
  return err;
}

int test_request10(flexid_t *id, response_t **ret)
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
  
  (*ret)->next_hop[0] = FIRST10;
  (*ret)->next_hop[1] = SECOND10;
  (*ret)->next_hop[2] = THIRD10;
  (*ret)->next_hop[3] = FOURTH10;
  (*ret)->next_hop[4] = FIFTH10;
  (*ret)->next_hop[5] = SIXTH10;

  return SUCCESS;

out:
  return err;
}

int test_request15(flexid_t *id, response_t **ret)
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
  
  (*ret)->next_hop[0] = FIRST15;
  (*ret)->next_hop[1] = SECOND15;
  (*ret)->next_hop[2] = THIRD15;
  (*ret)->next_hop[3] = FOURTH15;
  (*ret)->next_hop[4] = FIFTH15;
  (*ret)->next_hop[5] = SIXTH15;

  return SUCCESS;

out:
  return err;
}

int test_request1(flexid_t *id, response_t **ret)
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
  
  (*ret)->next_hop[0] = FIRST1;
  (*ret)->next_hop[1] = SECOND1;
  (*ret)->next_hop[2] = THIRD1;
  (*ret)->next_hop[3] = FOURTH1;
  (*ret)->next_hop[4] = FIFTH1;
  (*ret)->next_hop[5] = SIXTH1;

  return SUCCESS;

out:
  return err;
}

int test_request2(flexid_t *id, response_t **ret)
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
  
  (*ret)->next_hop[0] = FIRST2;
  (*ret)->next_hop[1] = SECOND2;
  (*ret)->next_hop[2] = THIRD2;
  (*ret)->next_hop[3] = FOURTH2;
  (*ret)->next_hop[4] = FIFTH2;
  (*ret)->next_hop[5] = SIXTH2;

  return SUCCESS;

out:
  return err;
}

int test_request9(flexid_t *id, response_t **ret)
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
  
  (*ret)->next_hop[0] = FIRST9;
  (*ret)->next_hop[1] = SECOND9;
  (*ret)->next_hop[2] = THIRD9;
  (*ret)->next_hop[3] = FOURTH9;
  (*ret)->next_hop[4] = FIFTH9;
  (*ret)->next_hop[5] = SIXTH9;

  return SUCCESS;

out:
  return err;
}
