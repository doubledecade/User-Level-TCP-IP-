//
// Created by iFOURYTWOLF on 2021/12/15.
//

#include <malloc.h>
#include <memory.h>
#include "skbuff.h"
struct sk_buff *alloc_skb(unsigned int size)
{
    struct sk_buff *skb = malloc(sizeof(struct sk_buff));

    memset(skb, 0, sizeof(struct sk_buff));
    skb->data = malloc(size);
    memset(skb->data, 0, size);

    skb->refcnt = 0;
    skb->head = skb->data;
    skb->end = skb->data + size;

    list_init(&skb->list);

    return skb;
}
void free_skb(struct sk_buff *skb)
{
    if (skb->refcnt < 1) {
        free(skb->head);
        free(skb);
    }
}