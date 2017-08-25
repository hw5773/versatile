#ifndef __FLEX_HEADER__
#define __FLEX_HEADER__
#endif

#define PROTOCOL_AUTHOR	"Hyunwoo Lee <hwlee2014@mmlab.snu.ac.kr>, Hyeonmin Lee <hmlee@mmlab.snu.ac.kr>, Dongjun Lee <djlee@mmlab.snu.ac.kr>, Hyunchul Oh <hcoh@mmlab.snu.ac.kr>"
#define PROTOCOL_DESC	"Flex Protocol"

#ifdef DEBUG
#define APP_LOG(msg) \
	printf("[Flex] %s\n", msg)
#else
#define APP_LOG(msg)
#endif

/* Address Family & Protocol Family 
 * 38 is collide with PF_ALG (if it is set, a Flex socket won't be set.)
 */
#define AF_FLEX			38
#define PF_FLEX			AF_FLEX

/* Ethernet Frame Type */
#define ETH_P_FLEX		0x7788

/* Flex Version */
#define FLEX_1_0        0x0100
#define FLEX_1_0_MAJOR  0x01
#define FLEX_1_0_MINOR  0x00

/* Flags */
#define FLEX_PTC        0x8000
#define FLEX_DF         0x4000
#define FLEX_MF         0x2000
#define FLEX_OFFSET     0x1FFF

/* Packet Type
 * begin with 00 is for control plane messages, 01 is for control plane ack
 * begin with ff is for data plane messages
 */

// Control Plane
#define FLEX_JOIN           0x0001  // Join the device with interfaces to Edge network
#define FLEX_STATUS         0x0002  // Update the status of interfaces
#define FLEX_LEAVE          0x0003  // Leave the device from Edge network
#define FLEX_REGISTER       0x0004  // Register the service/content/resource
#define FLEX_UPDATE         0x0005  // Update the service/content/resource
#define FLEX_QUERY          0x0006  // Query ID by keywords
#define FLEX_REQUEST        0x0007  // Request for resources

#define FLEX_JOIN_ACK       0x0101  // ACK for Join
#define FLEX_STATUS_ACK     0x0102  // ACK for Status
#define FLEX_LEAVE_ACK      0x0103  // ACK for Leave
#define FLEX_REGISTER_ACK   0x0104  // ACK for Register
#define FLEX_UPDATE_ACK     0x0105  // ACK for Update
#define FLEX_REPLY          0x0106  // Reply to Query binding by Packet ID?
#define FLEX_RESPONSE       0x0107  // Response for Request by Packet ID?

// Data Plane
#define FLEX_INTEREST       0xff01  // Interest for the service/content
#define FLEX_DATA           0xff02  // Data
#define FLEX_DATA_ACK       0xff03  // ACK for Data

/* Hash Type */
#define SHA1	            0x0001  // SHA1, 160 bits (20 bytes)
#define SHA192	          	0x0002  // 192 bits (24 bytes)
#define SHA224          	0x0003  // 224 bits (28 bytes)
#define SHA256          	0x0004  // 256 bits (32 bytes)
