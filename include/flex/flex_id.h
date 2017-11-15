#ifndef __FLEX_ID__
#define __FLEX_ID__

#include <flex/flex_types.h>

/* Flex ID */
struct flexid
{
  __u8    cache_bit:1,
          segment_bit:1,
          collision_avoidance_bit:4,
          reserved:2;
  __u8    identity[20];
  __be32  total_segments; // used only when segment_bit is 1
  __be32  segment_num;    // used only when segment_bit is 1
  __be16  length;
};

/* Flex ID init and free */
int init_flexid(flexid_t **id, void *buf, int type);
int free_flexid(flexid_t *id);

/* Flex ID Manipulation */
int set_cache_bit(flexid_t *id, int set);
int set_segment_bit(flexid_t *id, int set);
int set_collision_avoidance_bit(flexid_t *id, int set);
int add_attribute_value_pair(flexid_t *id, unsigned char *key, unsigned char *value);

/* Flex ID Print */
int print_flex_id(flexid_t *id);

#endif /* __FLEX_ID__ */
