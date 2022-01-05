//
// Created by AT on 2022/1/2.
//

#include "ip_input.h"
#include "skbuff.h"
#include "stdio.h"
#include "route.h"
#include "icmpv4.h"
//将网络字节序转成主机字节序
static void ip_init_pkt(struct iphdr *ih)
{
	ih->saddr = ntohl(ih->saddr);
	ih->daddr = ntohl(ih->daddr);
	ih->len = ntohs(ih->len);
	ih->id = ntohs(ih->id);
}

int ip_rcv(struct sk_buff *skb)
{
	struct iphdr *ih = ip_hdr(skb);
	uint16_t csum = -1;
	if(ih->version !=IPV4)
	{
		printf("协议版本不为 IPV4");
		goto drop_pkt;
	}
	if(ih->ihl<5)
	{
		printf("ipv4协议头长度至少为20");
		goto drop_pkt;
	}
	if(ih->ttl==0)
	{
		printf("生存周期已经到,丢弃该数据报");
		goto drop_pkt;
	}
	csum = checksum(ih, ih->ihl * 4, 0);
	if(csum!=0)
	{
		printf("校验和错误");
		goto drop_pkt;
	}
	ip_init_pkt(ih);
	switch (ih->proto) {
		case ICMPV4:
            icmpv4_incoming(skb);
			return 0;
		case IP_TCP:
			return 0;
		default:
			printf("未能识别ip头协议\n");
			goto drop_pkt;
	}
drop_pkt:
	free_skb(skb);

	return 0;
}


