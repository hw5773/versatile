#include "flex_sock.h"
#include "flex_utable.h"

struct u_table u_table __read_mostly;

/**
 * @brief Initialize the Flex ID network
 */
void __init flex_unreliable_init(void)
{
  FLEX_LOG("Start to initialize the unreliable one");
  u_table_init(&u_table, "Unreliable");
}

/**
 * @brief Destruct the Flex ID network
 */
void __exit flex_unreliable_exit(void)
{
  FLEX_LOG("Start to exit the unreliable one");
  u_table_exit(&u_table);
}

/**
 * @brief Initialize the unreliable socket table
 * @param table The unreliable socket table
 * @param name The name of the table
 */
void u_table_init(struct u_table *table, const char *name)
{
  int i;

  FLEX_LOG("Start to initialize the Unreliable Table");
  table->hash = kmalloc(MIN_UTABLE_SIZE * 2 * sizeof(struct u_hslot), GFP_KERNEL);
  if (!(table->hash))
  {
    FLEX_LOG("Initialize Unreliable Socket Hash Table Failure");
    panic(name);
  }
  table->log = ilog2(MIN_UTABLE_SIZE);
  table->mask = MIN_UTABLE_SIZE - 1;

  for (i=0; i<= table->mask; i++)
  {
    INIT_HLIST_NULLS_HEAD(&table->hash[i].head, i);
    table->hash[i].count = 0;
    spin_lock_init(&table->hash[i].lock);
  }
  FLEX_LOG("Allocate the table Succeed");
}

/**
 * @brief Destruct the unreliable socket table
 * @param table The unreliable table
 */
void u_table_exit(struct u_table *table)
{
  kfree(table->hash);
}

/**
 * @brief Look up the socket from the unreliable table by Flex ID
 * @param id Flex ID, the key for the socket
 * @param table The unreliable socket table
 * @return The socket related to the Flex ID
 */
struct sock *u_table_lookup_skb(flexid_t id, struct u_table *table)
{
  int err;
  struct sock *sk, *result;
  struct flex_sock *flex;
  struct u_hslot *hslot;
  struct hlist_nulls_node *node;
  unsigned int slot;

  slot = hash_fn(id, table->mask);
  hslot = &table->hash[slot];

  result = NULL;
  rcu_read_lock();
  sk_nulls_for_each_rcu(sk, node, &hslot->head)
  {
    flex = flex_sk(sk);
    if (flex->message == FLEX_INTEREST)
    {
      if (strncmp(flex->dst.identity, id.identity, sizeof(flexid_t)) == 0)
        result = sk;
    }
    else if (flex->message == FLEX_DATA)
    {
      if (strncmp(flex->src.identity, id.identity, sizeof(flexid_t)) == 0)
        result = sk;
    }
  }
  rcu_read_unlock();

  if (!result)
  {
    FLEX_LOG("Fail to find the appropriate socket.");
    err = -NO_SOCK;
    goto out;
  }

  FLEX_LOG("Find the appropriate socket.");

  return result;

out:
  return NULL;
}


