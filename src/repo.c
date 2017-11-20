/**
 * @file repo.c
 * @author Hyunwoo Lee
 * @date 20 Nov 2017
 * @brief This file is to define the interface for the repository
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <flex/flex.h>
#include <flex/flex_repo.h>

int urepo_sock;
struct hash_table urepo_table;

/**
 * @brief Initialize the socket for the repository
 */
int init_repo_sock()
{
  int err;

  err = -NO_SOCK;
  if ((urepo_sock = socket(PF_FLEX, SOCK_DGRAM, 0)) < 0) goto out;

  return SUCCESS;

out:
  return err;
}

/**
 * @brief Initialize the repository ID - file table
 */
int init_repo_table()
{
  hash_table_init(&urepo_table, 10, NULL);
  return SUCCESS;
}

/**
 * @brief Destruct the respository ID - file table
 */
int free_repo_table()
{
  hash_table_finit(&urepo_table);
  return SUCCESS;
}

/**
 * @brief Destruct the socket for the repository
 */
void free_repo()
{
  close(urepo_sock);
}

int add_id_name_map(flexid_t *id, unsigned char *name)
{
  int err;
  struct id_entry *tmp;

  APP_LOG("Add the Flex ID into the ID Table");
  APP_LOG1s("File name", name);

  err = -ERROR_MALLOC;
  if (!(tmp = (struct id_entry *)malloc(sizeof(struct id_entry)))) goto out;
  if (!(tmp->id = (flexid_t *)malloc(sizeof(flexid_t)))) goto out;

  APP_LOG1d("ID length", id->length);
  memcpy(tmp->id, id, id->length);
  APP_LOG("Copy id into tmp");
  memcpy(tmp->fn, name, strlen(name));
  APP_LOG("Copy file name into the hash entry");

  hash_table_insert(&urepo_table, &tmp->entry, (unsigned char *)id, id->length);

  APP_LOG("Add the Flex ID into the ID Table complete");

  return SUCCESS;

out:
  return err;
}

unsigned char *get_filename_by_id(flexid_t *id)
{
  int err;
  struct id_entry *tmp;
  struct hash_entry *hentry;
  unsigned char *result;
  
  result = (unsigned char *)malloc(MAX_FILE_PATH_SIZE);

  APP_LOG("Get the filename by Flex ID");

  err = -NO_ENTRY;
  if (!(hentry = hash_table_lookup_key(&urepo_table, (unsigned char *)id, sizeof(flexid_t)))) goto out;
  
  tmp = hash_entry(hentry, struct id_entry, entry);
  APP_LOG1s("File name", tmp->fn);

  memcpy(result, tmp->fn, MAX_FILE_PATH_SIZE);

  APP_LOG("Get the filename complete");

  return result;

out:
  free(result);
  return NULL;
}

