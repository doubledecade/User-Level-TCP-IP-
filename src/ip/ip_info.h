//
// Created by AT on 2022/1/2.
//

#ifndef USER_LEVEL_IP_IP_INFO_H
#define USER_LEVEL_IP_IP_INFO_H

#include <utils.h>
#include "skbuff.h"
#include "ethernet.h"
#define IPV4 0x04
#define IP_TCP 0x06
#define ICMPV4 0x01

#define IP_HDR_LEN sizeof(struct iphdr)
struct iphdr {
	uint8_t ihl : 4; /* TODO: Support Big Endian hosts */
	uint8_t version : 4;
	uint8_t tos;
	uint16_t len;
	uint16_t id;
	uint16_t frag_offset;
	uint8_t ttl;
	uint8_t proto;
	uint16_t csum;
	uint32_t saddr;
	uint32_t daddr;
	uint8_t data[];
} __attribute__((packed));

static inline uint32_t ip_info_parse(char * addr)
{
	return 0;
}
static inline struct iphdr*ip_hdr(const struct sk_buff *skb)
{
	return (struct iphdr*)(skb->head + ETH_HDR_LEN);
}

#endif //USER_LEVEL_IP_IP_INFO_H
