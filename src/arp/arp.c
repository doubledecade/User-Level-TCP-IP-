//
// Created by AT on 2022/1/3.
//

#include <pthread.h>
#include <list.h>
#include "arp.h"
#include "skbuff.h"
static LIST_HEAD(arp_cache);
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

unsigned char* arp_get_hwaddr(uint32_t sip)
{
	struct list_head *item;
	struct arp_cache_entry *entry;

	pthread_mutex_lock(&lock);
	list_for_each(item, &arp_cache) {
		entry = list_entry(item, struct arp_cache_entry, list);

		if (entry->state == ARP_RESOLVED &&
		    entry->sip == sip) {


			uint8_t *copy = entry->smac;
			pthread_mutex_unlock(&lock);

			return copy;
		}
	}

	pthread_mutex_unlock(&lock);

	return NULL;
}
void arp_rcv(struct sk_buff *skb)
{

}