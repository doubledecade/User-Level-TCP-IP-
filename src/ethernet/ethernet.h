//
// Created by MHP on 2021/12/15.
//

#ifndef USER_LEVEL_IP_ETHERNET_H
#define USER_LEVEL_IP_ETHERNET_H

#include <stdint.h>
#include <netinet/in.h>
#include "skbuff.h"

#define ETH_HDR_LEN sizeof(struct eth_hdr)
struct sk_buff;
struct netdev;

uint8_t *skb_head(struct sk_buff *skb);
struct eth_hdr
{
    uint8_t  dmac[6];
    uint8_t  smac[6];
    uint16_t ethertype;
    uint8_t  payload[];
} __attribute__((packed));

static inline struct  eth_hdr* eth_hdr(struct sk_buff *skb)
{
    struct eth_hdr *hdr =(struct eth_hdr*) skb_head(skb);
    hdr->ethertype=ntohs(hdr->ethertype);
    return hdr;
}
#endif //USER_LEVEL_IP_ETHERNET_H
