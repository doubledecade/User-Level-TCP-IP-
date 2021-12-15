//
// Created by MHP on 2021/12/15.
//

#include <malloc.h>
#include "netdev.h"
#include "utils.h"
struct netdev* loop;//回环地址
struct netdev* netdev;//网卡地址

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