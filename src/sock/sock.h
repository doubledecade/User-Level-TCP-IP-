//
// Created by AT on 2022/1/2.
//

#ifndef USER_LEVEL_IP_SOCK_H
#define USER_LEVEL_IP_SOCK_H
#include <stdint.h>
#include "skbuff.h"
struct net_ops {
	struct sock* (*alloc_sock) (int protocol);
	int (*init) (struct sock *sk);
//	int (*connect) (struct sock *sk, const struct sockaddr *addr, int addr_len, int flags);
	int (*disconnect) (struct sock *sk, int flags);
	int (*write) (struct sock *sk, const void *buf, int len);
	int (*read) (struct sock *sk, void *buf, int len);
	int (*recv_notify) (struct sock *sk);
	int (*close) (struct sock *sk);
	int (*abort) (struct sock *sk);
};

struct sock {
	struct socket *sock;
	struct net_ops *ops;
	//struct wait_lock recv_wait;
//	struct sk_buff_head receive_queue;
//	struct sk_buff_head write_queue;
	int protocol;
	int state;
	int err;
	short int poll_events;
	uint16_t sport;
	uint16_t dport;
	uint32_t saddr;
	uint32_t daddr;
};
#endif //USER_LEVEL_IP_SOCK_H
