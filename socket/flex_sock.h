#ifndef __FLEX_SOCK_H__
#define __FLEX_SOCK_H__

#include <linux/socket.h>
#include <linux/slab.h>
#include <net/sock.h>

#define PROTOCOL_AUTHOR	"Hyunwoo Lee <hwlee2014@mmlab.snu.ac.kr>, Hyeonmin Lee <hmlee@mmlab.snu.ac.kr>, Dongjun Lee <djlee@mmlab.snu.ac.kr>, Hyunchul Oh <hcoh@mmlab.snu.ac.kr>"
#define PROTOCOL_DESC	"Flex Protocol"

#include <flex/flex_log.h>
#include <flex/flex_const.h>
#include <flex/flex_id.h>
#include <flex/flex_hdr.h>
#include <flex/flex_socket.h>
#include <flex/flex_err.h>

/* Socket related functions */
struct flex_sock 
{
	struct sock	  sk;
  struct flexid dst;
  int           message;
};

/* Change the general sock into flex sock */
static inline struct flex_sock *flex_sk(const struct sock *sk)
{
	return (struct flex_sock *)sk;
}

/* flex_input.c */
extern int flex_rcv(struct sk_buff *, struct net_device *,
			struct packet_type *, struct net_device *);

/* flex_output.c */
extern int flex_output(struct sk_buff *);

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
extern int flex_unreliable_sendmsg(struct socket *, struct msghdr *, size_t len);
extern int flex_unreliable_sendmsg_test(struct socket *, struct msghdr *, size_t len);
extern int flex_unreliable_recvmsg(struct socket *, struct msghdr *, size_t, int);

#endif /* __FLEX_SOCK_H__ */
