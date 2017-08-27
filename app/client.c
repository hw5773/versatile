#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "proto_flex.h"

void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	struct flexhdr *flex;
	struct flexhdr *resp;
	char buf[256];
	int rc, len;

	char message[] = "{ \"interface\" : \"wire\", \"bandwidth\" : \"10 Mbps\" }";
	int message_len = strlen(message);

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

	APP_LOG("Send the JOIN message");

	rc = init_flex_header(&flex);

	if (!rc)
	{
		APP_LOG("Initialize the Flex Header Success");
	}
	else
	{
		APP_LOG("Initialize the Flex Header Failure");
		exit(1);
	}

    flex->version = FLEX_1_0;
    flex->packet_type = FLEX_JOIN;
    flex->hash_type = SHA1;
    flex->hop_limit = DEFAULT_HOP_LIMIT;
    flex->header_len = htons(DEFAULT_HEADER_LEN);
    flex->check = htons(0x1234);
    flex->packet_id = htons(0x7777);
    flex->frag_off = htons(0x8000 | 0x2000 | 0x365);
    memset(flex->sflex_id, '1', FLEX_ID_LENGTH);
    memset(flex->dflex_id, '7', FLEX_ID_LENGTH);
    flex->packet_len = htons(DEFAULT_HEADER_LEN + message_len);
    flex->seq = htonl(0x12345678);
    flex->ack = htonl(0x98765432);

	print_flex_header(flex);

	int packet_len = ntohs(flex->packet_len);

    memcpy(buf, flex, ntohs(flex->header_len));
    memcpy(buf + ntohs(flex->header_len), message, message_len);

    len = write(sock, buf, packet_len);
    printf("Sent Length: %d\n", len);
    APP_LOG("Send the message");

	len = read(sock, buf, sizeof(buf));
	if (len == -1)
		error_handling("read() error");
	
	printf("Recv Length: %d\n", len);
	printf("Message: %s\n", (buf + DEFAULT_HEADER_LEN));
	parse_flex_header(buf, len, &resp);
	print_flex_header(resp);

	free_flex_header(flex);
	free_flex_header(resp);

	close(sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
