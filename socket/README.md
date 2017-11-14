Note that the signature of sendmsg in proto\_ops in Linux 2.6 is different from that in Linux version above 3.0

In Linux 2.6:
	int (*sendmsg) (struct socket *sock, struct msghdr *m, size_t total_len);
	int (*recvmsg) (struct socket *sock, struct msghdr *m, size_t total_len, int flags);

In Linux above 3:
	int (*sendmsg) (struct kiocb *iocb, struct socket *sock, struct msghdr *m, size_t total_len);
	int (*recvmsg) (struct kiocb *iocb, struct socket *sock, struct msghdr *m, size_t total_len, int flags);


Description of Socket operations
connect:
  - binds the target id with the socket
