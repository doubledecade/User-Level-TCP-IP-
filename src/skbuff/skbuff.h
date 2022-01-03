//
// Created by iFOURYTWOLF on 2021/12/15.
//

#ifndef USER_LEVEL_IP_SKBUFF_H
#define USER_LEVEL_IP_SKBUFF_H

#include <stdint.h>
#include <list.h>

struct sk_buff {
    struct list_head list;
    struct rtentry *rt;
    struct netdev *dev;
    int refcnt;
    uint16_t protocol;
    uint32_t len;
    uint32_t dlen;
    uint32_t seq;
    uint32_t end_seq;
    uint8_t *end;
    uint8_t *head; //数据的开始位置
    uint8_t *data;
    uint8_t *payload;
};
struct sk_buff *alloc_skb(unsigned int size);
void free_skb(struct sk_buff *skb);
void *skb_reserve(struct sk_buff *skb, unsigned int len);
uint8_t *skb_push(struct sk_buff *skb, unsigned int len);
#endif //USER_LEVEL_IP_SKBUFF_H
