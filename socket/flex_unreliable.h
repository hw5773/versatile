#ifndef __FLEX_UNRELIABLE__
#define __FLEX_UNRELIABLE__

#include <linux/list.h>

#define MIN_UTABLE_SIZE 5

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

#endif /* __FLEX_UNRELIABLE__ */
