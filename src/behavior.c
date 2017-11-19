/**
 * @file behavior.c
 * @author Hyunwoo Lee
 * @date 17 Nov 2017
 * @brief This file is to define the interface for the application layer
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/* Flex ID Related Headers */
#include <flex/flex.h>
#include <flex/flex_const.h>
#include <flex/flex_log.h>
#include <flex/flex_socket.h>
#include <flex/flex_err.h>
#include <flex/flex_const.h>
#include <flex/flex_request.h>

/* OpenSSL Related Headers */
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

int urepo_sock;

/**
 * @brief Initialize the Flex ID networking application
 */
int init_flex()
{
  int err;
  if ((err = init_repo()) < 0) goto out;
  return SUCCESS;

out:
  return err;
}

/**
 * @brief Destruct the Flex ID networking application
 */
void free_flex()
{
  free_repo();
}

/**
 * @brief Initialize the socket for the repository
 */
int init_repo()
{
  int err;

  err = -NO_SOCK;
  if ((urepo_sock = socket(PF_FLEX, SOCK_DGRAM, 0)) < 0) goto out;

  return SUCCESS;

out:
  return err;
}

/**
 * @brief Destruct the socket for the repository
 */
void free_repo()
{
  close(urepo_sock);
}

/**
 * @brief Getting the content by Flex ID of the content
 * @param id Flex ID of the content
 * @param buf Buffer for the response
 * @param len Length of the buffer
 * @return Error code
 */
int get(flexid_t *id, char *buf, int *len)
{
  int sock, i, err;
  struct sockaddr_flex target_id;
  flexid_t *sid;    // TODO: Need to change this to be automated.
  response_t *resp;

  APP_LOG("Unreliable Get message");

  if ((err = test_request(id, &resp)) < 0) goto out;

  APP_LOG("Get Test Request");

  err = -NO_SOCK;
  if ((sock = socket(PF_FLEX, SOCK_DGRAM, 0)) < 0) goto out;

  APP_LOG("Socket Generation Succeed");
  
  target_id.sin_family = AF_FLEX;
  test_sid(&sid);
  target_id.sid = *sid;
  target_id.tid = *id;
  target_id.message = FLEX_INTEREST;

  APP_LOG("Set the Message Info to Target ID");

  target_id.addr_type = resp->addr_type;
  target_id.addr_len = resp->addr_len;
  for (i=0; i<resp->addr_len; i++)
    target_id.next_hop[i] = resp->next_hop[i];

  if (connect(sock, (struct sockaddr *)&target_id, sizeof(target_id)) == FAILURE)
    error_handling("connect() error");

  APP_LOG("Connect the Socket with the Target ID");

  APP_LOG("Send the INTEREST Message");

  if ((err = write(sock, NULL, 0)) < 0)
  {
    close(sock);
    goto out;
  }

  APP_LOG("Send the INTEREST Success");

  // TODO: Read

  free_response(resp);
  close(sock);
  return SUCCESS;

out:
  APP_LOG1d("Error", err);
  return err;
}

/**
 * @brief Sending the data to the entity with the Flex ID
 * @param id The Flex ID of the target
 * @param resp Buffer for the response
 * @param len Length of the buffer
 * @return Error code
 */
int put(flexid_t *id, char *resp, int *len)
{
  return SUCCESS;
}

/**
 * @brief Publishing the content with the name
 * @param name the file name of the content
 * @return Error code
 */
int pub(unsigned char *name)
{
  int err;
  struct sockaddr_flex insert_id;
  flexid_t *id;

  if ((err = init_flexid(&id, name, FLEX_TYPE_CONTENT)) < 0) goto out;
  APP_LOG("Make the Flex ID succeed");

  insert_id.sid = *id;
  insert_id.message = FLEX_DATA;

  err = -ERROR_BIND;
  if ((bind(urepo_sock, (struct sockaddr *)&insert_id, sizeof(insert_id))) < 0) goto out;

  //if ((err = add_id_name_map(&id, name)) < 0) goto out;

  return SUCCESS;

out:
  APP_LOG1d("Error", err);
  return err;
}

/**
 * @brief Serving the dynamic content
 * @param crt Certificate of the entity
 * @param key Private key corresponding to the Certificate
 * @return Error code
 */
int serv(unsigned char *crt, unsigned char *key)
{
  return SUCCESS;
}
