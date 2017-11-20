#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/skbuff.h>
#include <linux/scatterlist.h>
#include <linux/splice.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/random.h>
#include <linux/bootmem.h>
#include <linux/highmem.h>
#include <linux/swap.h>
#include <linux/cache.h>
#include <linux/err.h>
#include <linux/crypto.h>
#include <linux/time.h>
#include <linux/slab.h>

#include "flex_sock.h"
#include "flex_idtable.h"

int flex_release_sock(struct sock *sk, int embrion)
{
	FLEX_LOG("Release the socket internally");

	sk_del_node_init(sk);
	
	sock_set_flag(sk, SOCK_DEAD);
	sock_set_flag(sk, SOCK_DESTROY);

	FLEX_LOG("Invoke sock_orphan()");
	sock_orphan(sk);
	FLEX_LOG("Invoke release_sock()");
	release_sock(sk);
	FLEX_LOG("Invoke sock_put()");
	sock_put(sk);

	return 0;
}

int flex_release(struct socket *sock)
{
	struct sock *sk = sock->sk;
	FLEX_LOG("Release the socket");
	
	if (!sk)
		return 0;

	flex_release_sock(sk, 0);
	sock->sk = NULL;

	FLEX_LOG("Release the socket complete");

	return 0;
}

int flex_bind(struct socket *sock, struct sockaddr *addr, int addr_len)
{
  int err;
  flexid_t *id;
  struct sock *sk;
  struct sockaddr_flex *id_info;
	FLEX_LOG("Bind for Flex");

  id_info = target_info(addr);
  id = &id_info->sid;
  sk = sock->sk;

  if (!id_info)
  {
    FLEX_LOG("id_info null");
    goto out;
  }
  FLEX_LOG("id_info");

  if (!id)
  {
    FLEX_LOG("id null");
    goto out;
  }
  FLEX_LOG1p("id", id);

  if (!sk)
  {
    FLEX_LOG("sk null");
    goto out;
  }
  FLEX_LOG("sk");

  if (!(&id_table))
  {
    FLEX_LOG("id_table null");
    goto out;
  }

  if ((err = add_id_to_table(id, sk, &id_table)) < 0) goto out;
  FLEX_LOG("Add ID to socket map succeed");
	return SUCCESS;

out:
  return err;
}

int flex_getname(struct socket *sock, struct sockaddr *addr, int *addr_len, int peer)
{
	FLEX_LOG("Get the name");
	return -1;
}

int flex_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	FLEX_LOG("I/O Control for Flex");
	return -1;
}

int flex_shutdown(struct socket *sock, int how)
{
	FLEX_LOG("Shutdown");
	return -1;
}

int flex_setsockopt(struct socket *sock, int level, int optname,
        char __user *optval, unsigned int optlen)
{
	FLEX_LOG("Set the socket options");
	return -1;
}
int flex_getsockopt(struct socket *sock, int level, int optname,
        char __user *optval, int __user *optlen)
{
	FLEX_LOG("Get the socket options");
	return -1;
}
