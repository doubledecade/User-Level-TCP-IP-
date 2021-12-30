//
// Created by MHP on 2021/12/15.
//

#include "ethernet.h"
uint8_t *skb_head(struct sk_buff *skb)
{
    return skb->head;
}
