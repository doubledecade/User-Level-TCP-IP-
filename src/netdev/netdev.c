//
// Created by MHP on 2021/12/15.
//

#include <malloc.h>
#include <memory.h>
#include <tuntap.h>
#include "netdev.h"
#include "utils.h"
#include "ethernet.h"
#include "skbuff.h"
#include "ip_input.h"
#include "linux/if_tun.h"
#include "netdev.h"
#include "arp.h"
struct netdev* loop;//回环地址
struct netdev* netdev;//网卡地址
extern int running;
static struct netdev*netdev_alloc( char * addr, char * hwaddr,uint32_t mtu)
{
	struct netdev *dev = (struct netdev *)malloc(sizeof (struct netdev));
	dev->addr=ip_parse(addr);
	sscanf(hwaddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hwaddr[0],
	       &dev->hwaddr[1],
	       &dev->hwaddr[2],
	       &dev->hwaddr[3],
	       &dev->hwaddr[4],
	       &dev->hwaddr[5]);

	dev->addr_len = 6;
	dev->mtu = mtu;

	return dev;
}

void netdev_init(char *addr, char *hwaddr)
{
	loop = netdev_alloc("127.0.0.1", "00:00:00:00:00:00", 1500);
	netdev = netdev_alloc(addr, hwaddr, 1500);
}

void free_netdev()
{
	if(loop!=NULL)
	{
		free(loop);
	}
	if(netdev!=NULL)
	{
		free(netdev);
	}
}

//发送链路层数据包
int netdev_transmit(struct sk_buff *skb,uint8_t *dst_hw,uint16_t ethertype)
{
	struct netdev *dev;
	struct eth_hdr *hdr;
	int ret = 0;

	dev = skb->dev;

	skb_push(skb, ETH_HDR_LEN);

	hdr = (struct eth_hdr *)skb->data;

	memcpy(hdr->dmac, dst_hw, dev->addr_len);
	memcpy(hdr->smac, dev->hwaddr, dev->addr_len);

	hdr->ethertype = htons(ethertype);


	ret = tun_write((char *)skb->data, skb->len);

	return ret;
}
static int netdev_receive(struct sk_buff *skb)
{
    struct eth_hdr *hdr = eth_hdr(skb);
    //解析arp协议
    printf("协议类型 %x\n",hdr->ethertype);
	switch (hdr->ethertype) {

		case ETH_P_ARP:
			arp_rcv(skb);
			break;
		case ETH_P_IP:

			ip_rcv(skb);
			break;
		default:
			printf("未知协议类型");
			break;

	}
    return 0;
}

//解析模块
void * netdev_rx_loop()
{
    while (running){

        struct sk_buff *skb = alloc_skb(BUFLEN);

        if (tun_read((char *)skb->data, BUFLEN) < 0) {
            perror("ERR: Read from tun_fd");
            free_skb(skb);
            return NULL;
        }
        printf("%s\n",skb->data);
        netdev_receive(skb);

    }
    return NULL;
}
struct netdev* netdev_get(uint32_t sip)
{
    if (netdev->addr == sip) {
        return netdev;
    } else {
        return NULL;
    }
}

