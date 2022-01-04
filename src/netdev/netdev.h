//
// Created by MHP on 2021/12/15.
//

#ifndef USER_LEVEL_IP_NETDEV_H
#define USER_LEVEL_IP_NETDEV_H

#include <stdint.h>
struct sk_buff;
#define BUFLEN 1600
struct netdev {
	uint32_t addr; //网卡IP地址
	uint8_t addr_len; //网卡地址长度
	uint8_t hwaddr[6]; //网卡mac地址
	uint32_t mtu; //mtu大小
};
struct netdev* netdev_get(uint32_t sip);
void netdev_init(char *addr, char *hwaddr);
void free_netdev();
void * netdev_rx_loop();
int netdev_transmit(struct sk_buff *skb,uint8_t *dst_hw,uint16_t ethertype);
#endif //USER_LEVEL_IP_NETDEV_H
