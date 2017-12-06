#include <flex/flex_err.h>
#include <flex/flex_const.h>
#include <flex/flex_types.h>

#include "flex_sock.h"
#include "flex_idtable.h"
#include "flex_klog.h"

struct id_table id_table __read_mostly;

/**
 * @brief Initialize the ID Table
 * @param table The ID table
 * @param name The name of the ID table
 */
void id_table_init(struct id_table *table, const char *name)
{
  int i;

  FLEX_LOG("Start to initialize the Unreliable Table");
/*
  table->hash = alloc_large_system_hash(name, 2 * sizeof(struct id_hslot),
                                        idhash_entries,
                                        21,
                                        0,
                                        &table->log,
                                        &table->mask,
                                        MIN_IDTABLE_SIZE,
                                        64 * 1024);
*/
  
  table->hash = kmalloc(MIN_IDTABLE_SIZE * 2 * sizeof(struct id_hslot), GFP_ATOMIC);
  if (!(table->hash))
  {
    FLEX_LOG("Initialize Unreliable Socket Hash Table Failure");
    panic(name);
  }
  table->log = ilog2(MIN_IDTABLE_SIZE);
  table->mask = MIN_IDTABLE_SIZE - 1;

  for (i=0; i<= table->mask; i++)
  {
    INIT_HLIST_HEAD(&table->hash[i].head);
    table->hash[i].count = 0;
    spin_lock_init(&table->hash[i].lock);
  }
  FLEX_LOG("Allocate the table Succeed");
}

/**
 * @brief Destruct the ID table
 * @param table The ID table
 */
void id_table_exit(struct id_table *table)
{
/**
  int i;
  struct flexid_entity *flex;
  struct id_hslot *hslot;

  for (i=0; i<MIN_IDTABLE_SIZE; i++)
  {
    hslot = &table->hash[i];

    spin_lock_bh(&hslot->lock);
    hlist_for_each_entry_rcu(flex, &hslot->head, flex_node) 
    {
      hlist_del_init_rcu(&flex->flex_node);
    }
    spin_unlock_bh(&hslot->lock);
  }
  kfree(table->hash);
**/
}

/**
 * @brief Look up the ID from the ID table
 * @param id Flex ID
 * @param table The ID table
 * @return TRUE for existence, otherwise FALSE
 */
int id_exist(flexid_t *id, struct id_table *table)
{
  int result;
  struct flexid_entity *flex;
  struct id_hslot *hslot;
  unsigned int slot;

  slot = hash_fn(*id, table->mask);
  hslot = &table->hash[slot];

  result = FALSE;
  rcu_read_lock();
  hlist_for_each_entry_rcu(flex, &hslot->head, flex_node)
  {
    if (strncmp(flex->id->identity, id->identity, sizeof(IDENTITY_LENGTH)) == 0)
      result = TRUE;
  }
  rcu_read_unlock();

  return result;
}

/**
 * @brief Add the ID into the table
 * @param id Flex ID
 */
int add_id_to_table(flexid_t *id, struct sock *sk, struct id_table *table)
{
  int err;
  unsigned int slot;
  struct id_hslot *hslot;
  struct flexid_entity *flex;

  err = -ERROR_MALLOC;
  if (!(flex = (struct flexid_entity *)kmalloc(sizeof(struct flexid_entity), GFP_ATOMIC))) goto out;
  if (!(flex->id = (flexid_t *)kmalloc(sizeof(flexid_t), GFP_ATOMIC))) goto out;
  if (!(flex->sk = (struct sock *)kmalloc(sizeof(struct sock), GFP_ATOMIC))) goto out;
  FLEX_LOG("initialize flex id entity");
  memcpy(flex->id, id, sizeof(flexid_t));
  memcpy(flex->sk, sk, sizeof(struct sock));

  slot = hash_fn(*id, table->mask);

  hslot = &table->hash[slot];

  spin_lock_bh(&hslot->lock);
  hlist_add_head_rcu(&flex->flex_node, &hslot->head);
  hslot->count++;
  spin_unlock_bh(&hslot->lock);

  FLEX_LOG("Add the ID complete");

  return SUCCESS;

out:
  return FAILURE;
}

struct sock *get_sock_by_id(flexid_t *id, struct id_table *table)
{
  struct sock *result;
  struct flexid_entity *flex;
  struct id_hslot *hslot;
  unsigned int slot;

  slot = hash_fn(*id, table->mask);
  hslot = &table->hash[slot];
  num = hslot->count;
  result = NULL;

  if (num > 0)
  {
    rcu_read_lock();
    hlist_for_each_entry_rcu(flex, &hslot->head, flex_node)
    {
      num--;
      if (strncmp(flex->id->identity, id->identity, sizeof(IDENTITY_LENGTH)) == 0)
        result = flex->sk;

      if (num <= 0)
        break;
    }
    rcu_read_unlock();
  }
  
  return result;
}
