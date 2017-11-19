#include <stdlib.h>
#include <string.h>

/* Flex ID Related Headers */
#include <flex/flex_const.h>
#include <flex/flex_id.h>
#include <flex/flex_types.h>
#include <flex/flex_log.h>
#include <flex/flex_err.h>

/* OpenSSL Related Headers */
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

/**
 * @brief Initialize the Flex ID
 * @param id Flex ID
 * @param name Name of the file
 * @param type Type of the Flex ID (Service/Content)
 */
int init_flexid(flexid_t **id, unsigned char *name, int type)
{
  int err;
  FILE *fp;
  unsigned char *identity;

  APP_LOG("Make the Flex ID");

  err = -ERROR_INIT_ID;
  if (!((*id) = (flexid_t *)malloc(sizeof(flexid_t)))) goto out;

  err = -ERROR_MISTYPE;
  switch(type)
  {
    case FLEX_TYPE_CONTENT:
      set_cache_bit(*id, TRUE);
      set_segment_bit(*id, FALSE);
      set_collision_avoidance_bit(*id, FALSE);
      if ((err = content_identity(&identity, name)) < 0) goto out;
      memcpy((*id)->identity, identity, SHA_DIGEST_LENGTH);
      (*id)->length = FLEX_ID_LENGTH;
      break;
    case FLEX_TYPE_SEGMENT:
      set_cache_bit(*id, TRUE);
      set_segment_bit(*id, TRUE);
      set_collision_avoidance_bit(*id, FALSE);
      (*id)->length = FLEX_ID_EXT_LENGTH;
      break;
    case FLEX_TYPE_SERVICE:
      set_cache_bit(*id, FALSE);
      set_segment_bit(*id, FALSE);
      set_collision_avoidance_bit(*id, TRUE);
      service_identity(&identity, name);
      memcpy((*id)->identity, identity, SHA_DIGEST_LENGTH);
      (*id)->length = FLEX_ID_LENGTH;
      break;
    default:
      APP_LOG("Flex Type mismatch");
      goto out;
  }

  APP_LOG("Flex ID is made");

  return SUCCESS;

out:
  return err;
}

int free_flexid(flexid_t *id)
{
  free(id);
  return SUCCESS;
}

int content_identity(unsigned char **identity, unsigned char *name)
{
  int err, bytes, len;
  FILE *fp;
  unsigned char data[2];
  EVP_MD_CTX *ctx;

  APP_LOG("Make the Content Identity");

  err = -ERROR_MALLOC;
  if (!((*identity) = (unsigned char *)malloc(SHA_DIGEST_LENGTH))) goto out;

  err = -ERROR_READ_FILE;
  APP_LOG1s("File to be read", name);
  if (!(fp = fopen(name, "r"))) goto out;

  err = -ERROR_INIT_MD_CTX;
  if (!(ctx = EVP_MD_CTX_create())) goto out;
  
  err = -ERROR_INIT_MD;
  if (!EVP_DigestInit_ex(ctx, EVP_sha1(), NULL)) goto out;

  err = -ERROR_PROCESS_MD;
  while (bytes = fread(data, 1, 1, fp) != 0)
  {
    if (!EVP_DigestUpdate(ctx, data, bytes)) goto out;
  }

  err = -ERROR_FINAL_MD;
  if (!EVP_DigestFinal_ex(ctx, (*identity), &len)) goto out;

  APP_LOG1d("Length of Content Identity", len);

  return SUCCESS;

out:
  APP_LOG1d("Error", err);
  return err;
}

int service_identity(unsigned char **identity, unsigned char *name)
{
  return SUCCESS;
}

int set_cache_bit(flexid_t *id, int set)
{
  if (set == TRUE)
    id->cache_bit = 1;
  else if (set == FALSE)
    id->cache_bit = 0;

  return SUCCESS;
}

int set_segment_bit(flexid_t *id, int set)
{
  if (set == TRUE)
    id->segment_bit = 1;
  else if (set == FALSE)
    id->segment_bit = 0;
  else
    return FAILURE;

  return SUCCESS;
}

int set_collision_avoidance_bit(flexid_t *id, int set)
{
  if (set == TRUE)
    id->collision_avoidance_bit = 0xF;
  else if (set == FALSE)
    id->collision_avoidance_bit = 0x0;
  else
    return FAILURE;

  return SUCCESS;
}

int add_attribute_value_pair(flexid_t *id, unsigned char *key, unsigned char *value)
{
  //TODO: implement this function.
  return SUCCESS;
}
