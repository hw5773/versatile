#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "proto_flex.h"

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	int rc, len, packet_len;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	struct flexhdr *flex;
	struct flexhdr *resp;
	socklen_t clnt_addr_size;

	char buf[256] = "Test";

	int port;

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

	serv_sock = socket(PF_FLEX, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	APP_LOG("Socket Generation Succeed");

	if (write(serv_sock, buf, 4) < 0)
	{
		close(serv_sock);
		error_handling("write() error");
	}

	APP_LOG("After Write");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	APP_LOG("Set the socket address");

	if (bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
	{
		close(serv_sock);
		error_handling("bind() error");
	}
	
	APP_LOG("Bind the Socket with the Address");

	if (listen(serv_sock, 5) == -1)
	{
		close(serv_sock);
		error_handling("listen() error");
	}
	
	APP_LOG("Start Listening on the Port");

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

	APP_LOG("Accept the Client");

	if (clnt_sock == -1)
	{
		close(serv_sock);
		error_handling("accept() error");
	}

	close(clnt_sock);
	close(serv_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
