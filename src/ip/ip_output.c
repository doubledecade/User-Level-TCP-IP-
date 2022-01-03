//
// Created by AT on 2022/1/3.
//

#include "ip_output.h"
#include "stdio.h"
#include "skbuff.h"
#include "ip_info.h"
#include "route.h"
#include "sock.h"
#include "dst.h"
int ip_output(struct sock *sk, struct sk_buff *skb)
{
	struct rtentry *rt;
	struct iphdr *ihdr = ip_hdr(skb);

	rt = route_lookup(sk->daddr);

	if (!rt) {
		// TODO: dest_unreachable
		printf("IP output route lookup fail\n");
		return -1;
	}

	skb->dev = rt->dev;
	skb->rt = rt;

	skb_push(skb, IP_HDR_LEN);

	ihdr->version = IPV4;
	ihdr->ihl = 0x05;
	ihdr->tos = 0;
	ihdr->len = skb->len;
	ihdr->id = ihdr->id;
	ihdr->frag_offset = 0x4000;
	ihdr->ttl = 64;
	ihdr->proto = skb->protocol;
	ihdr->saddr = skb->dev->addr;
	ihdr->daddr = sk->daddr;
	ihdr->csum = 0;



	ihdr->len = htons(ihdr->len);
	ihdr->id = htons(ihdr->id);
	ihdr->daddr = htonl(ihdr->daddr);
	ihdr->saddr = htonl(ihdr->saddr);
	ihdr->csum = htons(ihdr->csum);
	ihdr->frag_offset = htons(ihdr->frag_offset);

	ip_send_check(ihdr);

	return dst_neigh_output(skb);
}
void ip_send_check(struct iphdr *ihdr)
{
	uint32_t csum = checksum(ihdr, ihdr->ihl * 4, 0);
	ihdr->csum = csum;
}
