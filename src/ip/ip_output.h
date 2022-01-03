//
// Created by AT on 2022/1/3.
//

#ifndef USER_LEVEL_IP_IP_OUTPUT_H
#define USER_LEVEL_IP_IP_OUTPUT_H
#include "ip_info.h"
#include "sock.h"
void ip_send_check(struct iphdr *ihdr);
int ip_output(struct sock *sk, struct sk_buff *skb);
#endif //USER_LEVEL_IP_IP_OUTPUT_H
