#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <flex/flex.h>

void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int sock;
	struct flexid tid;
	struct sockaddr_flex target_id;
	char buf[256];
	int rc, len;

	char message[] = "{ \"interface\" : \"wire\", \"bandwidth\" : \"10 Mbps\" }";
	int message_len = strlen(message);

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	APP_LOG("Start Flex ID Test Subscriber Application");

	sock = socket(PF_FLEX, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	APP_LOG("Socket Generation Succeed");

	memset(&target_id, 0, sizeof(target_id));
	target_id.sin_family = AF_FLEX;
	target_id.id = test_id();
	target_id.message = FLEX_INTEREST;

	APP_LOG("Set the Socket Address");

	if (connect(sock, (struct sockaddr*)&target_id, sizeof(target_id)) == -1)
		error_handling("connect() error");
	
	APP_LOG("Connect the socket with the target ID");

	APP_LOG("Send the INTEREST message");

	if (write(sock, message, strlen(message)) < 0)
	{
		close(sock);
		error_handling("write() error");
	}

	close(sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
