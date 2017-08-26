#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "proto_flex.h"

#define NUM_OF_CONTROL	7

char *message[3][NUM_OF_CONTROL] = 
{
	{	"Join",
		"Status",
		"Leave",
		"Register",
		"Update",
		"Query",
		"Request"
	},
	{	"Join ACK",
		"Status ACK",
		"Leave ACK",
		"Register ACK",
		"Update ACK",
		"Reply",
		"Response"
	},
	{	"Interest",
		"Data",
		"Data ACK"
	}
};

/* Initialize the Flex Header 
 * Output: Success(0), Error(Otherwise)
 */
int init_flex_header(struct flexhdr **flex)
{
	APP_LOG("Initialize Flex Header");
	int rc;
	(*flex) = (struct flexhdr *)malloc(sizeof(struct flexhdr));

	if ((*flex))
	{
		APP_LOG("Initialize Success");
		rc = SUCCESS;
	}
	else
	{
		APP_LOG("Initialize Failure");
		rc = FAILURE;
	}

	memset((*flex)->sflex_id, 0, FLEX_ID_LENGTH);
	memset((*flex)->dflex_id, 0, FLEX_ID_LENGTH);

	return rc;
}

/* Process the Flex Packet
 * Output: Success(0), Error(otherwise)
 */
int parse_flex_header(char *hdr, int hdr_len, struct flexhdr **flex)
{
	int rc = SUCCESS;

	APP_LOG("Parse the buffer");

	if (hdr_len < sizeof(struct flexhdr))
	{
		rc = FAILURE;
		goto out;
	}
	
	(*flex) = (struct flexhdr *)malloc(sizeof(struct flexhdr));

	P_ONE(hdr, (*flex)->version);
	P_ONE(hdr, (*flex)->packet_type);
	P_ONE(hdr, (*flex)->hash_type);
	P_ONE(hdr, (*flex)->hop_limit);
	P_TWO(hdr, (*flex)->header_len);
	P_TWO(hdr, (*flex)->check);
	P_TWO(hdr, (*flex)->packet_id);
	P_TWO(hdr, (*flex)->frag_off);
	P_NCOPY(hdr, (*flex)->sflex_id, FLEX_ID_LENGTH);
	P_NCOPY(hdr, (*flex)->dflex_id, FLEX_ID_LENGTH);
	P_TWO(hdr, (*flex)->packet_len);
	P_FOUR(hdr, (*flex)->seq);
	P_FOUR(hdr, (*flex)->ack);	

	printf("Message in parse: %s\n", hdr);
	
out:
	return rc;
}

/* Print the Flex Header */
void print_flex_header(struct flexhdr *flex)
{
	int i, major_type, minor_type;

	APP_LOG("Flex Header Log");

	// Print Protocol Version
	switch (flex->version)
	{
	case FLEX_1_0:
		printf("Version: Flex 1.0\n");
		break;
	default:
		printf("Version: Error\n");
	}

	// Print Packet Type
	major_type = ((flex->packet_type) & 0xf0) >> 8;
	minor_type = ((flex->packet_type) & 0x0f) - 1;
	printf("Packet Type: %s\n", message[major_type][minor_type]);

	// Print Hash Type
	switch (flex->hash_type)
	{
	case SHA1:
		printf("Hash Type: SHA1\n");
		break;
	case SHA192:
		printf("Hash Type: SHA192\n");
		break;
	case SHA224:
		printf("Hash Type: SHA224\n");
		break;
	case SHA256:
		printf("Hash Type: SHA256\n");
		break;
	default:
		printf("Hash Type: Error\n");
	}
	
	printf("Hop Limit: %d\n", flex->hop_limit);
	printf("Header Length: %d\n", ntohs(flex->header_len));
	printf("Checksum: %x\n", ntohs(flex->check));
	printf("Packet ID: %x\n", ntohs(flex->packet_id));
	printf("PTC: %d\n", ((ntohs(flex->frag_off) & 0x8000) >> 15));
	printf("Don't Fragment: %d\n", ((ntohs(flex->frag_off) & 0x4000) >> 14));
	printf("More Fragment: %d\n", ((ntohs(flex->frag_off) & 0x2000) >> 13));
	printf("Fragment Offset: %d\n", (ntohs(flex->frag_off) & 0x1fff));

	// Print Source Flex ID
	printf("Source Flex ID: ");
	for (i=0; i<FLEX_ID_LENGTH; i++)
		printf("%x", (flex->sflex_id)[i]);
	printf("\n");

	// Print Destination Flex ID
	printf("Destination Flex ID: ");
	for (i=0; i<FLEX_ID_LENGTH; i++)
		printf("%x", (flex->dflex_id)[i]);
	printf("\n");

	printf("Packet Length: %d\n", ntohs(flex->packet_len));
	printf("Sequence: %x\n", ntohl(flex->seq));
	printf("Acknowledgement: %x\n", ntohl(flex->ack));
}
