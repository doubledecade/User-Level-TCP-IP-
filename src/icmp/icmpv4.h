//
// Created by AT on 2022/1/2.
//

#ifndef USER_LEVEL_IP_ICMPV4_H
#define USER_LEVEL_IP_ICMPV4_H

#define ICMP_V4_REPLY           0x00
#define ICMP_V4_DST_UNREACHABLE 0x03
#define ICMP_V4_SRC_QUENCH      0x04
#define ICMP_V4_REDIRECT        0x05
#define ICMP_V4_ECHO            0x08
#define ICMP_V4_ROUTER_ADV      0x09
#define ICMP_V4_ROUTER_SOL      0x0a
#define ICMP_V4_TIMEOUT         0x0b
#define	ICMP_TSTAMP		13		/* timestamp request */
#define	ICMP_TSTAMPREPLY	14		/* timestamp reply */


struct icmp_v4 {
	uint8_t type;
	uint8_t code;
	uint16_t csum;
	uint8_t data[];
} __attribute__((packed));

struct icmp_v4_timestamp {
    uint8_t type;
    uint8_t code;
    uint16_t csum;
    uint16_t icd_id;
    uint16_t icd_seq;
    uint32_t icmp_otime;
    uint32_t icmp_rtime;
    uint32_t icmp_ttime;
} __attribute__((packed));
void icmpv4_reply(struct sk_buff *skb);
void icmpv4_incoming(struct sk_buff *skb);
void icmpv4_timestamp_reply(struct sk_buff *skb);
uint32_t iptime();
#endif //USER_LEVEL_IP_ICMPV4_H
