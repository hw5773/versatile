/* Flex ID operation */
struct flexid_ops
{
  int (*set_cache_bit)(int set);
  int (*set_segment_bit)(int set);
  int (*set_collision_avoidance)(int set);
  int (*add_attribute_value_pair)(unsigned char *key, unsigned char *value);
};

/* Flex ID */
struct flexid
{
  __be16  length;
  __u8    cache_bit:1,
          segment_bit:1,
          collision_avoidance_bit:4,
          reserved:2;
  __u8    identity[20];
  __be32  total_segments; // used only when segment_bit is 1
  __be32  segment_num;    // used only when segment_bit is 1
  struct flexid_ops *ops;
};

int init_flexid(struct flexid **);
int free_flexid(struct flexid *);
