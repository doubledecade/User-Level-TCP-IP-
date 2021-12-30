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
int netdev_transmit(struct netdev *netdata,uint8_t *data,uint8_t *dst_hw,uint16_t ethertype)
{
    struct eth_hdr *hdr;
    hdr = (struct eth_hdr *)data;
    int ret =0;
    //复制链路层的mac地址数据
    memcpy(hdr->dmac, dst_hw, netdata->addr_len);
    memcpy(hdr->smac, netdata->hwaddr, netdata->addr_len);
    //赋值链路层标志位
    hdr->ethertype = htons(ethertype);

    ret = tun_write((char *)data, strlen(data));
    return ret;
}
static int netdev_receive(struct sk_buff *skb)
{
    struct eth_hdr *hdr = eth_hdr(skb);
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

