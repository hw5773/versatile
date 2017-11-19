#ifndef __FLEX_UNRELIABLE__
#define __FLEX_UNRELIABLE__

#include <linux/list.h>

#define MIN_UTABLE_SIZE 4

struct id_hslot {
  struct hlist_nulls_head head;
  int count;
  spinlock_t  lock;
} __attribute__((aligned(2 * sizeof(long))));

struct id_table {
  struct id_hslot  *hash;
  unsigned int    mask;
  unsigned int    log;
};

extern struct id_table id_table;
extern void id_table_init(struct id_table *, const char *name);
extern void id_table_exit(struct id_table *);
extern int id_table_exist(flexid_t id, struct id_table *table);
#endif /* __FLEX_UNRELIABLE__ */
