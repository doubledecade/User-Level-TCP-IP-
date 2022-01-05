//
// Created by AT on 2022/1/2.
//

#include <ip_info.h>
#include <memory.h>
#include <sys/time.h>
#include "icmpv4.h"
#include "utils.h"
#include "stdio.h"
#include "sock.h"
#include "ip_output.h"
void icmpv4_incoming(struct sk_buff *skb)
{
	struct iphdr* iphdr=ip_hdr(skb);
	struct icmp_v4 *icmp =(struct icmp_v4*)iphdr->data;
	uint16_t csum = -1;
	uint16_t icmp_len = iphdr->len - (iphdr->ihl * 4);
	csum = checksum(icmp,icmp_len,0);
	if(csum!=0)
	{
		printf("校验和错误");
		goto drop_pkt;
	}
    printf("icmp 协议类型是%x\n",icmp->type);
	switch (icmp->type) {
		case ICMP_V4_ECHO:
			icmpv4_reply(skb);
			return;
        case ICMP_TSTAMP:
            icmpv4_timestamp_reply(skb);
            return;
		default:
			printf("未知类型");
			goto drop_pkt;
	}

drop_pkt:
	free_skb(skb);
	return;
}

void icmpv4_timestamp_reply(struct sk_buff *skb)
{
    struct iphdr *iphdr = ip_hdr(skb);
    struct icmp_v4_timestamp *icmp;
    struct sock sk;
    memset(&sk, 0, sizeof(struct sock));
    uint16_t icmp_len = iphdr->len - (iphdr->ihl * 4);

    skb_reserve(skb, ETH_HDR_LEN + IP_HDR_LEN + icmp_len);
    skb_push(skb, icmp_len);
    icmp = (struct icmp_v4_timestamp *)skb->data;
    icmp->type=ICMP_TSTAMPREPLY;
    icmp->csum = 0;

    icmp->icmp_rtime=iptime();

    icmp->icmp_ttime=icmp->icmp_rtime;
    icmp->csum = checksum(icmp, icmp_len, 0);
    skb->protocol = ICMPV4;
    sk.daddr = iphdr->saddr;

    ip_output(&sk, skb);

    free_skb(skb);
}

void icmpv4_reply(struct sk_buff *skb)
{
	struct iphdr *iphdr = ip_hdr(skb);
	struct icmp_v4 *icmp;
	struct sock sk;
	memset(&sk, 0, sizeof(struct sock));
	uint16_t icmp_len = iphdr->len - (iphdr->ihl * 4);
	skb_reserve(skb, ETH_HDR_LEN + IP_HDR_LEN + icmp_len);
	skb_push(skb, icmp_len);
	icmp = (struct icmp_v4 *)skb->data;
	icmp->type = ICMP_V4_REPLY;
	icmp->csum = 0;
	icmp->csum = checksum(icmp, icmp_len, 0);

	skb->protocol = ICMPV4;
	sk.daddr = iphdr->saddr;

	ip_output(&sk, skb);
	free_skb(skb);
}
uint32_t iptime()
{
    struct timeval atv;
    u_long t;
    gettimeofday(&atv, NULL);

    t = (atv.tv_sec % (24*60*60)) * 1000 + atv.tv_usec / 1000;
    return (htonl(t));
}
