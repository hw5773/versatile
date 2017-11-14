#include <stdlib.h>
#include <flex/flex_const.h>
#include <flex/flex_id.h>
#include <flex/flex_types.h>
#include <flex/flex_log.h>

struct flexid *test_id()
{
  flexid_t *id;
  if (init_flexid(&id) < 0)
    return NULL;

  return id;
}

// TODO: Extract public key from the DER/PEM file
// TODO: Make the hash value from the content
int init_flexid(flexid_t **id, void *buf, int type)
{
  (*id) = (flexid_t *)malloc(sizeof(flexid_t));

  if (!(*id))
    return FAILURE;

  if (type == FLEX_TYPE_CONTENT)
  {
    set_cache_bit(id, TRUE);
    set_segment_bit(id, FALSE);
    set_collision_avoidance_bit(id, FALSE);
  }
  else if (type == FLEX_TYPE_SEGMENT)
  {
    set_cache_bit(id, TRUE);
    set_segment_bit(id, TRUE);
    set_collision_avoidance_bit(id, FALSE);
  }
  else if (type == FLEX_TYPE_SERVICE)
  {
    set_cache_bit(id, FALSE);
    set_segment_bit(id, FALSE);
    set_collision_avoidance_bit(id, TRUE);
  }
  else
  {
    APP_LOG("Flex Type mismatch");
    return FAILURE;
  }

  return SUCCESS;
}

int free_flexid(flexid_t *id)
{
  free(id);
  return SUCCESS;
}

int set_cache_bit(flexid_t *id, int set)
{
  if (set == TRUE)
    id->cache_bit = 1;
  else if (set == FALSE)
    id->cache_bit = 0;

  return SUCCESS;
}

int set_segment_bit(flexid_t *id, int set)
{
  if (set == TRUE)
    id->segment_bit = 1;
  else if (set == FALSE)
    id->segment_bit = 0;
  else
    return FAILURE;

  return SUCCESS;
}

int set_collision_avoidance(flexid_t *id, int set)
{
  if (set == TRUE)
    id->collision_avoidance_bit = 0xF;
  else if (set == FALSE)
    id->collision_avoidance_bit = 0x0;
  else
    return FAILURE;

  return SUCCESS;
}

int add_attribute_value_pair(flexid_t *id, unsigned char *key, unsigned char *value)
{
  //TODO: implement this function.
  return SUCCESS;
}
