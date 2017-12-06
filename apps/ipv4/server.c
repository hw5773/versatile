#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <flex/flex.h>
#include <flex/flex_repo.h>

#define FLEX_PORT 3333

//#include "proto_flex.h"

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	int i, rc, len, fsize;
  int err;

  unsigned char *fn, *content;
  unsigned char a = 0x80;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	socklen_t clnt_addr_size;

	char buf[256];

	int port;

  flexid_t *tid;
  flexid_t id;
  FILE *fp;

	if (argc == 1)
	{
		port = FLEX_PORT;
	} 
	else if (argc == 2)
	{
		port = atoi(argv[1]);
	}
	else
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	APP_LOG("Start Flex ID Test Server Application");

  if ((err = init_repo_table()) < 0) goto out;

  err = -ERROR_MALLOC;
  if (!(tid = (flexid_t *)malloc(sizeof(flexid_t)))) goto out;

  set_cache_bit(tid, TRUE);
  set_segment_bit(tid, FALSE);
  set_collision_avoidance_bit(tid, FALSE);
/*
  for (i=0; i<10; i++)
    tid->identity[i] = 0x41;

  for (i=10; i<20; i++)
    tid->identity[i] = 0x42;
*/
  tid->identity[0] = 0x40;
  tid->identity[1] = 0xf0;
  tid->identity[2] = 0x67;
  tid->identity[3] = 0xc1;
  tid->identity[4] = 0xcd;
  tid->identity[5] = 0x83;
  tid->identity[6] = 0x96;
  tid->identity[7] = 0x2a;
  tid->identity[8] = 0xbf;
  tid->identity[9] = 0x35;
  tid->identity[10] = 0x99;
  tid->identity[11] = 0xde;
  tid->identity[12] = 0x79;
  tid->identity[13] = 0xc1;
  tid->identity[14] = 0xab;
  tid->identity[15] = 0xb7;
  tid->identity[16] = 0x7b;
  tid->identity[17] = 0x2f;
  tid->identity[18] = 0x02;
  tid->identity[19] = 0x6d;

  tid->length = FLEX_ID_LENGTH;

  APP_LOG("Set the Test Flex ID");

  err = -ERROR_ADD_ID;
  if ((err = add_id_name_map(tid, TEST_FILE_PATH)) < 0) goto out;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	APP_LOG("Socket Generation Succeed");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	APP_LOG("Set the socket address");

	if (bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	
	APP_LOG("Bind the Socket with the Address");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	APP_LOG("Start Listening on the Port");

  while (1)
  {
	  clnt_addr_size = sizeof(clnt_addr);
	  clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

	  APP_LOG("Accept the Client");

	  if (clnt_sock < -1)
		  error_handling("accept() error");

	  len = read(clnt_sock, buf, sizeof(buf));
	  if (len == -1)
		  error_handling("read() error");
	
	  APP_LOG1d("Recv Length", len);
    APP_LOG1s("Received", buf);
    memcpy(&id, buf, len);
    id.length = FLEX_ID_LENGTH;
    APP_LOG2s("identity2", id.identity, id.length - 1);

    fn = get_filename_by_id(&id);
    APP_LOG1s("File name", fn);
    fp = fopen(fn, "rb");
    fseek(fp, 0L, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    APP_LOG1d("File length", fsize);

    content = (unsigned char *)malloc(fsize + 1);
    fread(content, fsize, 1, fp);
    len = fsize;

	  write(clnt_sock, content, len);
    close(clnt_sock);
  }

	close(serv_sock);

	return 0;

out:
  APP_LOG1d("Error", err);
  return err;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
