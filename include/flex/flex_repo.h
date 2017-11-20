#ifndef __FLEX_REPO__
#define __FLEX_REPO__

#include <flex/flex_id.h>
#include <flex/list.h>
#include <flex/hash_table.h>

struct id_entry
{
  flexid_t *id;             /**< Flex ID */
  unsigned char fn[256];    /**< The file name corresponding to the Flex ID */
  struct hash_entry entry;  /**< The pointer to the next entry */
};

int init_repo_sock(void);
int init_repo_table(void);
void free_repo(void);
int add_id_name_map(flexid_t *id, unsigned char *name);
#endif /* __FLEX_REPO__ */
