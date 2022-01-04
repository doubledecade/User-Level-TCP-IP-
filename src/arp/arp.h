//
// Created by AT on 2022/1/3.
//

#ifndef USER_LEVEL_IP_ARP_H
#define USER_LEVEL_IP_ARP_H

#define ARP_CACHE_LEN   32
#define ARP_FREE        0
#define ARP_WAITING     1
#define ARP_RESOLVED    2
#include <stdint.h>
#include "skbuff.h"
#include "ethernet.h"
#define ARP_ETHERNET    0x0001
#define ARP_IPV4        0x0800
#define ARP_REQUEST     0x0001
#define ARP_REPLY       0x0002

#define ARP_HDR_LEN sizeof(struct arp_hdr)
#define ARP_DATA_LEN sizeof(struct arp_ipv4)

#define ARP_CACHE_LEN   32
#define ARP_FREE        0
#define ARP_WAITING     1
#define ARP_RESOLVED    2

struct sk_buff;
struct arp_cache_entry
{
	struct list_head list;
	uint16_t hwtype;
	uint32_t sip;
	unsigned char smac[6];
	unsigned int state;
};
struct arp_hdr
{
	uint16_t hwtype;
	uint16_t protype;
	uint8_t hwsize;
	uint8_t prosize;
	uint16_t opcode;
	unsigned char data[];
} __attribute__((packed));

struct arp_ipv4
{
	unsigned char smac[6];
	uint32_t sip;
	unsigned char dmac[6];
	uint32_t dip;
} __attribute__((packed));

unsigned char* arp_get_hwaddr(uint32_t sip);
void arp_rcv(struct sk_buff *skb);
static inline struct arp_hdr *arp_hdr(struct sk_buff *skb)
{
    return (struct arp_hdr *)(skb->head + ETH_HDR_LEN);
}
#endif //USER_LEVEL_IP_ARP_H
