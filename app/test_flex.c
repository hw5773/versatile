#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "proto_flex.h"

int main()
{
	struct flexhdr *flex;
	struct flexhdr *test;
	char *buf;
	int rc;

	const char *test_message = "{ \"interface\" : \"wifi\", \"bandwidth\" : \"10 Mbps\" }";
	int message_len = strlen(test_message);

	APP_LOG("Start Flex Header Program");
	printf("Test Message: %s\n", test_message);

	rc = init_flex_header(&flex);

	if (!rc)
	{
		APP_LOG("Initialize the Flex Header Success");
	}
	else
	{
		APP_LOG("Initialize the Flex Header Failure\n");
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
	memset(flex->sflex_id, 1, FLEX_ID_LENGTH);
	memset(flex->dflex_id, 7, FLEX_ID_LENGTH);
	flex->packet_len = htons(DEFAULT_HEADER_LEN + message_len);
	flex->seq = htonl(0x12345678);
	flex->ack = htonl(0x09876543);

	print_flex_header(flex);

	printf("struct size: %lu\n", sizeof(struct flexhdr));
	printf("header size: %d\n", flex->header_len);

	buf = (char *)malloc(flex->packet_len);
	memcpy(buf, flex, flex->header_len);
	memcpy(buf + flex->header_len, test_message, message_len);
	printf("Message: %s\n", buf + flex->header_len);

	rc = parse_flex_header(buf, flex->packet_len, &test);

	print_flex_header(test);

	APP_LOG("Print Test Message");
	printf("Test Message: %s\n", buf + test->header_len);

	return 0;
}
