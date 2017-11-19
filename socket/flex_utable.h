#ifndef __FLEX_UNRELIABLE__
#define __FLEX_UNRELIABLE__

#include <linux/list.h>

#define MIN_UTABLE_SIZE 4

struct u_hslot {
  struct hlist_nulls_head head;
  int count;
  spinlock_t  lock;
} __attribute__((aligned(2 * sizeof(long))));

struct u_table {
  struct u_hslot  *hash;
  unsigned int    mask;
  unsigned int    log;
};

extern struct u_table u_table;
extern void u_table_init(struct u_table *, const char *name);
extern void u_table_exit(struct u_table *);
extern struct sock *u_table_lookup_skb(flexid_t id, struct u_table *table);

static inline int hash_fn(flexid_t id, unsigned int mask)
{
  return ((unsigned int) id.identity) & mask;
}

#endif /* __FLEX_UNRELIABLE__ */
