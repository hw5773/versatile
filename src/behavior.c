#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Flex ID Related Headers
#include <flex/flex.h>
#include <flex/flex_const.h>
#include <flex/flex_log.h>
#include <flex/flex_socket.h>
#include <flex/flex_err.h>
#include <flex/flex_const.h>
#include <flex/flex_request.h>

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

int put(flexid_t *id, char *resp, int *len)
{
  return SUCCESS;
}

int pub(unsigned char *name)
{
  return SUCCESS;
}

int serv(unsigned char *crt, unsigned char *key)
{
  return SUCCESS;
}
