//
// Created by AT on 2022/2/17.
//

#ifndef USER_LEVEL_IP_IP_FRAGMENT_H
#define USER_LEVEL_IP_IP_FRAGMENT_H
#include <ip_info.h>
struct ipq	 {
	unsigned char		*mac;		/* pointer to MAC header		*/
	struct iphdr	*iph;		/* pointer to IP header			*/
	int		len;		/* total length of original datagram	*/
	short			ihlen;		/* length of the IP header		*/
	short 	maclen;		/* length of the MAC header		*/
	struct timer_list timer;	/* when will this queue expire?		*/
	struct ipfrag		*fragments;	/* linked list of received fragments	*/
	struct ipq	*next;		/* linked list pointers			*/
	struct ipq	*prev;
	struct device *dev;		/* Device - for icmp replies */
};
struct ipfrag {
	int		offset;		/* offset of fragment in IP datagram	*/
	int		end;		/* last byte of data in datagram	*/
	int		len;		/* length of this fragment		*/
	struct sk_buff *skb;			/* complete received fragment		*/
	unsigned char		*ptr;		/* pointer into real fragment data	*/
	struct ipfrag		*next;		/* linked list pointers			*/
	struct ipfrag		*prev;
};
#endif //USER_LEVEL_IP_IP_FRAGMENT_H
