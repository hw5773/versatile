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
  unsigned char a = 0x80;
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

	APP_LOG("Start TCP/IP with Flex ID Test Client Application");

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

//	APP_LOG("Socket Generation Succeed");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

//	APP_LOG("Set the Socket Address");

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	
//	APP_LOG("Connect to the Server");
  
  err = -ERROR_MALLOC;
  if (!(id = (flexid_t *)malloc(sizeof(flexid_t)))) goto out;
/*
  set_cache_bit(id, TRUE);
  set_segment_bit(id, FALSE);
  set_collision_avoidance_bit(id, FALSE);
*/

  memcpy(id, &a, 1);
  id->identity[0] = 0x40;
  id->identity[1] = 0xf0;
  id->identity[2] = 0x67;
  id->identity[3] = 0xc1;
  id->identity[4] = 0xcd;
  id->identity[5] = 0x83;
  id->identity[6] = 0x96;
  id->identity[7] = 0x2a;
  id->identity[8] = 0xbf;
  id->identity[9] = 0x35;
  id->identity[10] = 0x99;
  id->identity[11] = 0xde;
  id->identity[12] = 0x79;
  id->identity[13] = 0xc1;
  id->identity[14] = 0xab;
  id->identity[15] = 0xb7;
  id->identity[16] = 0x7b;
  id->identity[17] = 0x2f;
  id->identity[18] = 0x02;
  id->identity[19] = 0x6d;

/*
  for (i=0; i<10; i++)
    id->identity[i] = 0x41;

  for (i=10; i<20; i++)
    id->identity[i] = 0x42;
*/
  id->length = FLEX_ID_LENGTH;

  APP_LOG("Set the Content ID for Retreiving the value of the fine dust Complete");
  memcpy(buf, id, id->length);

  start = get_current_microseconds();
  len = write(sock, buf, id->length);

  while (1)
  {
	  if ((len = read(sock, buf, sizeof(buf))) > 0)
    {
//      APP_LOG1d("Read bytes", len);
      APP_LOG2s("Result", buf, len);
      break;
    }
  }
  end = get_current_microseconds();

	if (len == -1)
		error_handling("read() error");
	
  APP_LOG1lu("Elapsed Time for get()", (end - start));

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
