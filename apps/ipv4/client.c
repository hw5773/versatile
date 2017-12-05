#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <flex/flex.h>

#define FLEX_PORT 3333

//#include "proto_flex.h"

void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int sock, err;
	struct sockaddr_in serv_addr;
	struct flexhdr *flex;
	struct flexhdr *resp;
	char buf[256];
	int i, rc, len;
  flexid_t *id;
  unsigned long start, end;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	APP_LOG("Start Flex ID Test Client Application");

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	APP_LOG("Socket Generation Succeed");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	APP_LOG("Set the Socket Address");

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	
	APP_LOG("Connect to the Server");
  
  err = -ERROR_MALLOC;
  if (!(id = (flexid_t *)malloc(sizeof(flexid_t)))) goto out;

  set_cache_bit(id, TRUE);
  set_segment_bit(id, FALSE);
  set_collision_avoidance_bit(id, FALSE);

  for (i=0; i<10; i++)
    id->identity[i] = 0x41;

  for (i=10; i<20; i++)
    id->identity[i] = 0x42;

  id->length = FLEX_ID_LENGTH;

  APP_LOG("Set the Test Flex ID");
  memcpy(buf, id, id->length);

  start = get_current_microseconds();
  len = write(sock, buf, id->length);

  while (1)
  {
	  if ((len = read(sock, buf, sizeof(buf))) > 0)
    {
      APP_LOG1d("Read bytes", len);
      APP_LOG2s("Result", buf, len);
      break;
    }
  }
  end = get_current_microseconds();

	if (len == -1)
		error_handling("read() error");
	
  APP_LOG1s("Received", buf);
  APP_LOG1lu("Elapsed Time", (end - start));

	close(sock);
	return 0;

out:
  APP_LOG1d("Error", err);
  return err;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
