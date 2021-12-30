//
// Created by MHP on 2021/12/28.
//

#include <malloc.h>
#include "route.h"
#include "netdev.h"
#include "utils.h"
//定义一个双向链表指向自己
static LIST_HEAD(routes);
extern struct netdev *netdev;
extern struct netdev *loop;

//申请空间并赋值
static struct rtentry *route_alloc(uint32_t dst, uint32_t gateway, uint32_t netmask,
                                   uint8_t flags, uint32_t metric, struct netdev *dev)
{
    struct rtentry *rt = malloc(sizeof(struct rtentry));
    list_init(&rt->list);

    rt->dst = dst;
    rt->gateway = gateway;
    rt->netmask = netmask;
    rt->flags = flags;
    rt->metric = metric;
    rt->dev = dev;
    return rt;
}

void route_add(uint32_t dst, uint32_t gateway, uint32_t netmask, uint8_t flags,
               uint32_t metric, struct netdev *dev)
{
    struct rtentry *rt = route_alloc(dst, gateway, netmask, flags, metric, dev);

    list_add_tail(&rt->list, &routes);
}

void route_init()
{
    route_add(loop->addr, 0, 0xff000000, RT_LOOPBACK, 0, loop);
    route_add(netdev->addr, 0, 0xffffff00, RT_HOST, 0, netdev);
    route_add(0, ip_parse("10.0.0.5"), 0, RT_GATEWAY, 0, netdev);
}

struct rtentry *route_lookup(uint32_t daddr)
{
    struct list_head *item;
    struct rtentry *rt = NULL;

    list_for_each(item, &routes) {
        rt = list_entry(item, struct rtentry, list);
        if ((daddr & rt->netmask) == (rt->dst & rt->netmask)) break;
        // If no matches, we default to to default gw (last item)
    }

    return rt;
}

void free_routes()
{
    struct list_head *item, *tmp;
    struct rtentry *rt;

    list_for_each_safe(item, tmp, &routes) {
        rt = list_entry(item, struct rtentry, list);
        list_del(item);

        free(rt);
    }
}