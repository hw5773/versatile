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

  err = test_request(id, &resp);

  APP_LOG("Get Test Request");

  if (err < 0)
  {
    APP_LOG("Error in request()");
    err = -NO_RESP;
    goto out;
  }

  sock = socket(PF_FLEX, SOCK_DGRAM, 0);
  if (sock == -1)
    error_handling("socket() error");

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
  int err, bytes, len;
  flexid_t *id;

  if ((err = init_flexid(&id, name, FLEX_TYPE_CONTENT)) < 0) goto out;
  printf("Make the Flex ID succeed\n");

  return SUCCESS;

out:
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
