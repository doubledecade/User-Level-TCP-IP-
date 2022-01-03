//
// Created by AT on 2022/1/3.
//

#include <ip_info.h>
#include <route.h>
#include <linux/if_tun.h>
#include "dst.h"
#include "arp.h"
int dst_neigh_output(struct sk_buff *skb)
{
	struct iphdr *iphdr = ip_hdr(skb);
	struct netdev *netdev = skb->dev;
	struct rtentry *rt = skb->rt;
	uint32_t daddr = ntohl(iphdr->daddr);
	uint32_t saddr = ntohl(iphdr->saddr);

	uint8_t *dmac;

	if (rt->flags & RT_GATEWAY) {
		daddr = rt->gateway;
	}

	dmac = arp_get_hwaddr(daddr);

	if (dmac) {
		return netdev_transmit(skb, dmac, ETH_P_IP);
	} else {
		//TODO:缓存没有该mac地址发送arp请求
//		arp_request(saddr, daddr, netdev);

		/* Inform upper layer that traffic was not sent, retry later */
		return -1;
	}
}