#ifndef __FLEX_CONST__
#define __FLEX_CONST__

#define SUCCESS				0
#define FAILURE				-1

#define TRUE          0
#define FALSE         -1

/* Address Family & Protocol Family 
 * 38 is collide with PF_ALG (if it is set, a Flex socket won't be set.)
 */
#define AF_FLEX				38
#define PF_FLEX				AF_FLEX

/* Ethernet Frame Type for Flex ID */
#define ETH_P_FLEX			0x7788

/* Flex Type */
#define FLEX_TYPE_CONTENT 1
#define FLEX_TYPE_SEGMENT 2
#define FLEX_TYPE_SERVICE 3     

/* Flex Reliable/Unreliable */
#define FLEX_RELIABLE     1       // Used in struct flex_sock
#define FLEX_UNRELIABLE   0       // Used in struct flex_sock

/* Flex Version */
#define FLEX_1_0        	0x10
#define FLEX_1_0_MAJOR  	0x1
#define FLEX_1_0_MINOR  	0x0

/* Flags */
#define FLEX_PTC_RELIABLE    	0x8000  // PTC is set to 1
#define FLEX_PTC_UNRELIABLE   0x0000  // PTC is set to 0
#define FLEX_DF         	    0x4000
#define FLEX_MF         	    0x2000
#define FLEX_OFFSET  	   	    0x1FFF

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
#define FLEX_SHA1           0x01  // SHA1, 160 bits (20 bytes)
#define FLEX_SHA192        	0x02  // 192 bits (24 bytes)
#define FLEX_SHA224        	0x03  // 224 bits (28 bytes)
#define FLEX_SHA256        	0x04  // 256 bits (32 bytes)

/* Length Constant */
#define FLEX_ID_LENGTH		        21
#define FLEX_ID_EXT_LENGTH        29
#define DEFAULT_HOP_LIMIT	        128
#define DEFAULT_HEADER_LEN	      64
#define RELIABLE_HEADER_LEN       64
#define RELIABLE_EXT_HEADER_LEN   80
#define UNRELIABLE_HEADER_LEN     56
#define UNRELIABLE_EXT_HEADER_LEN 72

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

/* Flag Parser */
#define GET_FLEX_PTC(frag) \
  (frag & FLEX_PTC_RELIABLE) >> 15

#define GET_FLEX_DF(frag) \
  (frag & FLEX_DF) >> 14

#define GET_FLEX_MF(frag) \
  (frag & FLEX_MF) >> 13

#endif /* __FLEX_CONST__ */
