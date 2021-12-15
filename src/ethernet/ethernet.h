//
// Created by MHP on 2021/12/15.
//

#ifndef USER_LEVEL_IP_ETHERNET_H
#define USER_LEVEL_IP_ETHERNET_H

#include <stdint.h>

struct eth_hdr
{
    uint8_t  dmac[6];
    uint8_t  smac[6];
    uint16_t ethertype;
    uint8_t  payload[];
} __attribute__((packed));
#endif //USER_LEVEL_IP_ETHERNET_H
