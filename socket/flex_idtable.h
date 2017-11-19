#ifndef __FLEX_UNRELIABLE__
#define __FLEX_UNRELIABLE__

#include <linux/list.h>

#define MIN_IDTABLE_SIZE 4

struct flexid_entity {
  flexid_t *id;
  struct sock *sk;
  struct hlist_node *flex_node;
};

struct id_hslot {
  struct hlist_head head;
  int count;
  spinlock_t  lock;
} __attribute__((aligned(2 * sizeof(long))));

struct id_table {
  struct id_hslot  *hash;
  unsigned int    mask;
  unsigned int    log;
};

extern struct id_table id_table;
extern void id_table_init(struct id_table *table, const char *name);
extern void id_table_exit(struct id_table *table);
extern int id_exist(flexid_t *id, struct id_table *table);
extern int add_id_to_table(flexid_t *id, struct sock *sk, struct id_table *table);
extern struct sock *get_sock_by_id(flexid_t *id, struct id_table *table);

static inline int hash_fn(flexid_t id, unsigned int mask)
{
  return ((unsigned int) id.identity) & mask;
}
#endif /* __FLEX_UNRELIABLE__ */
