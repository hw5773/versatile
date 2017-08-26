#ifndef __FLEX_HEADER__
#define __FLEX_HEADER__
#endif

#include <linux/types.h>

#define PROTOCOL_AUTHOR	"Hyunwoo Lee <hwlee2014@mmlab.snu.ac.kr>, Hyeonmin Lee <hmlee@mmlab.snu.ac.kr>, Dongjun Lee <djlee@mmlab.snu.ac.kr>, Hyunchul Oh <hcoh@mmlab.snu.ac.kr>"
#define PROTOCOL_DESC	"Flex Protocol"

#ifdef DEBUG
#define APP_LOG(msg) \
	printf("[Flex] %s: %s\n", __func__, msg)
#else
#define APP_LOG(msg)
#endif

#define SUCCESS				0
#define FAILURE				-1

/* Address Family & Protocol Family 
 * 38 is collide with PF_ALG (if it is set, a Flex socket won't be set.)
 */
#define AF_FLEX				38
#define PF_FLEX				AF_FLEX

/* Ethernet Frame Type */
#define ETH_P_FLEX			0x7788

/* Default port for Flex ID over TCP/IP */
#define FLEX_PORT			1234

/* Flex Version */
#define FLEX_1_0        	0x10
#define FLEX_1_0_MAJOR  	0x1
#define FLEX_1_0_MINOR  	0x0

/* Flags */
#define FLEX_PTC        	0x8000
#define FLEX_DF         	0x4000
#define FLEX_MF         	0x2000
#define FLEX_OFFSET     	0x1FFF

/* Packet Type
 * begin with 00 is for control plane messages, 01 is for control plane ack
 * begin with ff is for data plane messages
 */

// Control Plane
#define FLEX_JOIN           0x01  // Join the device with interfaces to Edge network
#define FLEX_STATUS         0x02  // Update the status of interfaces
#define FLEX_LEAVE          0x03  // Leave the device from Edge network
#define FLEX_REGISTER       0x04  // Register the service/content/resource
#define FLEX_UPDATE         0x05  // Update the service/content/resource
#define FLEX_QUERY          0x06  // Query ID by keywords
#define FLEX_REQUEST        0x07  // Request for resources

#define FLEX_JOIN_ACK       0x11  // ACK for Join
#define FLEX_STATUS_ACK     0x12  // ACK for Status
#define FLEX_LEAVE_ACK      0x13  // ACK for Leave
#define FLEX_REGISTER_ACK   0x14  // ACK for Register
#define FLEX_UPDATE_ACK     0x15  // ACK for Update
#define FLEX_REPLY          0x16  // Reply to Query binding by Packet ID?
#define FLEX_RESPONSE       0x17  // Response for Request by Packet ID?

// Data Plane
#define FLEX_INTEREST       0x21  // Interest for the service/content
#define FLEX_DATA           0x22  // Data
#define FLEX_DATA_ACK       0x23  // ACK for Data

/* Hash Type */
#define SHA1	            0x01  // SHA1, 160 bits (20 bytes)
#define SHA192	          	0x02  // 192 bits (24 bytes)
#define SHA224          	0x03  // 224 bits (28 bytes)
#define SHA256          	0x04  // 256 bits (32 bytes)

#define FLEX_ID_LENGTH		21
#define DEFAULT_HOP_LIMIT	128
#define DEFAULT_HEADER_LEN	64

/* Packet Processing Macro */
#define P_ONE(buf, val)			val = ((unsigned char)(*((buf)++))) & 0xff
#define P_TWO(buf, val)			(val = ((unsigned char)(*((buf)++)) & 0xff), \
								val |= (((unsigned char)(*((buf)++)) & 0xff) << 8))
#define P_FOUR(buf, val)		(val = ((unsigned char)(*((buf)++)) & 0xff), \
								val |= (((unsigned char)(*((buf)++)) & 0xff) << 8), \
								val |= (((unsigned char)(*((buf)++)) & 0xff) << 16), \
								val |= (((unsigned char)(*((buf)++)) & 0xff) <<24))
#define P_NCOPY(buf, val, n)	memcpy(val, buf, n); buf = buf + n

/* Flex Header Field Index */
#define VERSION_IDX			0
#define PACKET_TYPE_IDX		1
#define HASH_TYPE_IDX		2
#define HOP_LIMIT_IDX		3
#define HEADER_LEN_IDX		4
#define CHECK_IDX			6
#define PACKET_ID_IDX		8
#define FRAG_OFF_IDX		10
#define SFLEX_ID_IDX		12
#define DFLEX_ID_IDX		12 + FLEX_ID_LENGTH
#define PACKET_LEN_IDX		12 + 2 * FLEX_ID_LENGTH
#define SEQ_IDX				12 + 2 * FLEX_ID_LENGTH + 2
#define ACK_IDX				12 + 2 * FLEX_ID_LENGTH + 6

/* Flex Header */
struct flexhdr {
	__u8	version;
	__u8	packet_type;
	__u8	hash_type;
	__u8	hop_limit;
	__be16	header_len;
	__sum16	check;
	__be16	packet_id;
	__be16	frag_off;
	char	sflex_id[FLEX_ID_LENGTH];
	char	dflex_id[FLEX_ID_LENGTH];
	__be16	packet_len;
	__be32	seq;
	__be32	ack;
};

int init_flex_header(struct flexhdr **flex);
int free_flex_header(struct flexhdr *flex);
int parse_flex_header(char *hdr, int hdr_len, struct flexhdr **flex);
void print_flex_header(struct flexhdr *flex);
