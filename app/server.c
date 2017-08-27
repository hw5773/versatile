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

	char buf[256];
	char message[] = "{ \"error\" : \"0\"}";
	int message_len = strlen(message);

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

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

	APP_LOG("Accept the Client");

	if (clnt_sock == -1)
		error_handling("accept() error");

	APP_LOG("Receive the Join message");
	len = read(clnt_sock, buf, sizeof(buf));
	if (len == -1)
		error_handling("read() error");
	
	printf("Recv Length: %d\n", len);
	printf("Message: %s\n", (buf + DEFAULT_HEADER_LEN));
	parse_flex_header(buf, len, &flex);
	print_flex_header(flex);

	APP_LOG("Generate the Join ACK message");

	rc = init_flex_header(&resp);

	if (!rc)
	{
		APP_LOG("Initialize the Flex Header Success");
	}
	else
	{
		APP_LOG("Initialize the Flex Header Failure\n");
		exit(1);
	}

	resp->version = FLEX_1_0;
	resp->packet_type = FLEX_JOIN_ACK;
	resp->hash_type = SHA1;
	resp->hop_limit = DEFAULT_HOP_LIMIT;
	resp->header_len = htons(DEFAULT_HEADER_LEN);
	resp->check = htons(0x1234);
	resp->packet_id = htons(0x7777);
	resp->frag_off = htons(0x8000 | 0x2000 | 0x365);
	memset(resp->sflex_id, '7', FLEX_ID_LENGTH);
	memset(resp->dflex_id, '1', FLEX_ID_LENGTH);
	resp->packet_len = htons(DEFAULT_HEADER_LEN + message_len);
	resp->seq = htonl(0x98765431);
	resp->ack = htonl(0x12345678 + ntohs(flex->packet_len));

	print_flex_header(resp);

	packet_len = ntohs(resp->packet_len);

	memcpy(buf, resp, ntohs(resp->header_len));
	memcpy(buf + ntohs(resp->header_len), message, message_len);

	APP_LOG("Send the message");
	len = write(clnt_sock, buf, packet_len);
	printf("Sent Length: %d\n", len);

	free_flex_header(flex);
	free_flex_header(resp);

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
