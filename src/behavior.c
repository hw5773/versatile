#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <flex/flex.h>
#include <flex/flex_log.h>
#include <flex/flex_socket.h>

int get(flexid_t *id, char *resp, int *len)
{
  int sock;
  struct sockaddr_flex target_id;

  APP_LOG("Unreliable Get message");
  sock = socket(PF_FLEX, SOCK_DGRAM, 0);
  if (sock == -1)
    error_handling("socket() error");

  APP_LOG("Socket Generation Succeed");
  
  target_id.sin_family = AF_FLEX;
  target_id.id = *id;
  target_id.message = FLEX_INTEREST;

  APP_LOG("Set the Message Info to Target ID");

  if (connect(sock, (struct sockaddr *)&target_id, sizeof(target_id)) == FAILURE)
    error_handling("connect() error");

  APP_LOG("Connect the Socket with the Target ID");

  APP_LOG("Send the INTEREST Message");

  if (write(sock, NULL, 0) < 0)
  {
    close(sock);
    return FAILURE;
  }

  APP_LOG("Send the INTEREST Success");

  close(sock);
  return SUCCESS;
}

int put(flexid_t *id, char *resp, int *len)
{
  return SUCCESS;
}
