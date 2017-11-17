#ifndef __FLEX_ID__
#define __FLEX_ID__

#include <linux/types.h>
#include <asm/byteorder.h>

/* Flex ID */
struct flexid
{
#if defined(__LITTLE_ENDIAN_BITFIELD)
  __u8    cache_bit:1,
          segment_bit:1,
          collision_avoidance_bit:4,
          reserved:2;
#elif defined(__BIG_ENDIAN_BITFIELD)
  __u8    reserved:2,
          collision_avoidance_bit:4,
          segment_bit:1,
          cache_bit:1;
#endif
  __u8    identity[20];
  __be32  total_segments; // used only when segment_bit is 1
  __be32  segment_num;    // used only when segment_bit is 1
  __be16  length;
};

/* Flex ID init and free */
int init_flexid(struct flexid **id, unsigned char *name, int type);
int free_flexid(struct flexid *id);
int service_identity(unsigned char **, unsigned char *);
int content_identity(unsigned char **, unsigned char *);

/* Flex ID Manipulation */
int set_cache_bit(struct flexid *id, int set);
int set_segment_bit(struct flexid *id, int set);
int set_collision_avoidance_bit(struct flexid *id, int set);
int add_attribute_value_pair(struct flexid *id, unsigned char *key, unsigned char *value);

#endif /* __FLEX_ID__ */
