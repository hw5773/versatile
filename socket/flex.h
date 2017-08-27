#ifndef __FLEX_H__
#define __FLEX_H__
#endif

#include <linux/socket.h>
#include <linux/slab.h>
#include <net/sock.h>

#define PROTOCOL_AUTHOR	"Hyunwoo Lee <hwlee2014@mmlab.snu.ac.kr>, Hyeonmin Lee <hmlee@mmlab.snu.ac.kr>, Dongjun Lee <djlee@mmlab.snu.ac.kr>, Hyunchul Oh <hcoh@mmlab.snu.ac.kr>"
#define PROTOCOL_DESC	"Flex Protocol"

#define FLEX_LOG(msg) \
	printk(KERN_INFO "[Flex] %s: %s\n", __func__, msg)

#define SUCCESS				0
#define FAILURE				-1

/* Address Family & Protocol Family 
 * 38 is collide with PF_ALG (if it is set, a Flex socket won't be set.)
 */
#define AF_FLEX				38
#define PF_FLEX				AF_FLEX

/* Ethernet Frame Type for Flex ID */
#define ETH_P_FLEX			0x7788

/* Flex Version */
#define FLEX_1_0        	0x0100
#define FLEX_1_0_MAJOR  	0x01
#define FLEX_1_0_MINOR  	0x00

/* Flags */
#define FLEX_PTC        	0x8000
#define FLEX_DF         	0x4000
#define FLEX_MF         	0x2000
#define FLEX_OFFSET  	   	0x1FFF

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

/* Socket related functions */
struct flex_sock {
	struct sock		sk;
};

static inline struct flex_sock *flex_sk(const struct sock *sk)
{
	return (struct flex_sock *)sk;
}

/* flex_input.c */
extern int flex_rcv(struct sk_buff *, struct net_device *,
			struct packet_type *, struct net_device *);

/* flex_common.c */
extern int flex_release(struct socket *);
extern int flex_bind(struct socket *, struct sockaddr *, int);
extern int flex_getname(struct socket *, struct sockaddr *, int *, int);
extern int flex_ioctl(struct socket *, unsigned int, unsigned long);
extern int flex_shutdown(struct socket *, int);
extern int flex_setsockopt(struct socket *, int level, int optname, 
		char __user *optval, unsigned int optlen);
extern int flex_getsockopt(struct socket *, int level, int optname, 
		char __user *optval, int __user *optlen);

/* flex_reliable.c */
extern int flex_reliable_connect(struct socket *, struct sockaddr *, int addr_len, int flags);
extern int flex_accept(struct socket *, struct socket *, int);
extern unsigned int flex_reliable_poll(struct file *, struct socket *, poll_table *);
extern int flex_listen(struct socket *, int backlog);
extern int flex_reliable_sendmsg(struct socket *, struct msghdr *, size_t);
extern int flex_reliable_recvmsg(struct socket *, struct msghdr *, size_t, int);

/* flex_unreliable.c */
extern int flex_unreliable_connect(struct socket *, struct sockaddr *, int addr_len, int flags);
extern unsigned int flex_unreliable_poll(struct file *, struct socket *, poll_table *);
extern int flex_unreliable_sendmsg(struct socket *, struct msghdr *, size_t);
extern int flex_unreliable_recvmsg(struct socket *, struct msghdr *, size_t, int);
