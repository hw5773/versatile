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
#include <flex/flex_repo.h>
#include <flex/flex_tools.h>

/* OpenSSL Related Headers */
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

int urepo_sock;
struct hash_table repo_table;

/**
 * @brief Initialize the Flex ID networking application
 */
int init_flex()
{
  int err;
  if ((err = init_repo_sock()) < 0) goto out;
  if ((err = init_repo_table()) < 0) goto out;
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

int start_repo()
{
  int rcvd, len;
  unsigned long fsize;
  unsigned char buf[BUF_SIZE];
  unsigned char *fn;
  unsigned char *content;
  flexid_t id1, id2;
  FILE *fp;

  while (1)
  {
    if ((rcvd = read(urepo_sock, buf, BUF_SIZE)) >= 0)
    {
      APP_LOG1d("Read bytes", rcvd);
      memcpy(&id1, buf, rcvd / 2);
      memcpy(&id2, buf + rcvd / 2, rcvd / 2);
      APP_LOG2s("identity received", buf, rcvd);
      id1.length = rcvd / 2;
      id2.length = rcvd / 2;
      fn = get_filename_by_id(&id2);
      APP_LOG1s("File name", fn);
      fp = fopen(fn, "rb");
      fseek(fp, 0L, SEEK_END);
      fsize = ftell(fp);
      fseek(fp, 0L, SEEK_SET);

      APP_LOG1d("File length", fsize);

      content = (unsigned char *)malloc(fsize + 1);
      fread(content, fsize, 1, fp);
      len = fsize;

      put(&id1, &id2, content, &len);
    }
  }

  return SUCCESS;
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
  // TODO: Need to reflect buf
  int sock, i, err, rcvd;
  struct sockaddr_flex target_id;
  flexid_t *sid;    // TODO: Need to change this to be automated.
  response_t *resp;

  APP_LOG("Unreliable Get message");

  if ((err = test_request1(id, &resp)) < 0) goto out;

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
  APP_LOG2s("sid identity", sid->identity, sid->length - 1);
  APP_LOG2s("tid identity", id->identity, id->length - 1);

  target_id.addr_type = resp->addr_type;
  target_id.addr_len = resp->addr_len;
  for (i=0; i<resp->addr_len; i++)
    target_id.next_hop[i] = resp->next_hop[i];

  if ((err = connect(sock, (struct sockaddr *)&target_id, sizeof(target_id))) < 0) goto out;

  APP_LOG("Connect the Socket with the Target ID");

  APP_LOG("Send the INTEREST Message");

  if ((err = write(sock, NULL, 0)) < 0) goto out;

  APP_LOG("Send the INTEREST Success");

  while (1)
  {
    if ((rcvd = read(sock, buf, BUF_SIZE)) >= 0)
    {
      APP_LOG1d("Read bytes", rcvd);
      APP_LOG1s("Result", buf);
      break;
    }
  }

  free_response(resp);
  close(sock);
  return rcvd;

out_write:
  close(sock);
out:
  APP_LOG1d("Error", err);
  return err;
}

/**
 * @brief Sending the data to the entity with the Flex ID
 * @param id The Flex ID of the target
 * @param resp Buffer to be sent
 * @param len Length of message sent
 * @return Error code
 */
int put(flexid_t *tid, flexid_t *sid, char *buf, int *len)
{
  int sock, i, err;
  struct sockaddr_flex target_id;
  response_t *resp;

  APP_LOG("Unreliable Put message");

  APP_LOG1s("Check the content", buf);

  if ((err = test_request2(tid, &resp)) < 0) goto out;

  err = -NO_SOCK;
  if ((sock = socket(PF_FLEX, SOCK_DGRAM, 0)) < 0) goto out;

  APP_LOG("Socket Generation Succeed");

  target_id.sin_family = AF_FLEX;
  target_id.sid = *sid;
  target_id.tid = *tid;
  target_id.message = FLEX_DATA;

  APP_LOG("Set the Message Info to Target ID");
  APP_LOG2s("sid identity", sid->identity, sid->length - 1);
  APP_LOG2s("tid identity", tid->identity, tid->length - 1);

  target_id.addr_type = resp->addr_type;
  target_id.addr_len = resp->addr_len;
  for (i=0; i<resp->addr_len; i++)
    target_id.next_hop[i] = resp->next_hop[i];

  if ((err = connect(sock, (struct sockaddr *)&target_id, sizeof(target_id))) < 0) goto out;

  APP_LOG("Connect the Socket with the Target ID");

  APP_LOG("Send the DATA Message");

  if ((err = write(sock, buf, *len)) < 0) goto out_write;

  APP_LOG("Send the DATA Success");

  return SUCCESS;

out_write:
  close(sock);
out:
  APP_LOG1d("Error", err);
  return err;
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
  unsigned char *path;
  flexid_t *id;

  if ((err = get_full_path(name, &path)) < 0) goto out;

  if ((err = init_flexid(&id, path, FLEX_TYPE_CONTENT)) < 0) goto out;
  APP_LOG("Make the Flex ID succeed");

  insert_id.sid = *id;
  insert_id.message = FLEX_DATA;
  APP_LOG1p("id", id);

  err = -ERROR_BIND;
  if ((bind(urepo_sock, (struct sockaddr *)&insert_id, sizeof(insert_id))) < 0) goto out;

  APP_LOG("Bind the socket with the Flex ID(s)");

  if ((err = add_id_name_map(id, name)) < 0) goto out;

  APP_LOG("Add the ID with the name complete");

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
