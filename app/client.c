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
	int sock;
	struct sockaddr_in serv_addr;
	struct flexhdr *flex;
	char message[256];
	int str_len;

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

	str_len = read(sock, message, sizeof(message));
	if (str_len == -1)
		error_handling("read() error");
	
	printf("Recv Length: %d\n", str_len);
	printf("Message: %s\n", (message + DEFAULT_HEADER_LEN));
	parse_flex_header(message, str_len, &flex);
	print_flex_header(flex);

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
