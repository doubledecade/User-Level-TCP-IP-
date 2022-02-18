//
// Created by AT on 2022/2/17.
//

#include "ip_fragment.h"
#include "skbuff.h"
#include "netinet/in.h"
#include "ip_info.h"
static struct ipq *ipqueue = NULL;		/* ip分片队列	*/

/**
 STI(Set Interrupt) 中断标志置1指令 使 IF = 1
CLI(Clear Interrupt) 中断标志置0指令 使 IF = 0

即：STI 打开中断，CLI关闭中断
 * */
static struct ipq *ip_find(struct iphdr *iph)
{
	struct ipq *qp;
	struct ipq *qplast;

	//cli();
	qplast = NULL;
	for(qp = ipqueue; qp != NULL; qplast = qp, qp = qp->next)
	{	// 对比ip头里的几个字段
		if (iph->id== qp->iph->id && iph->saddr == qp->iph->saddr &&
		    iph->daddr == qp->iph->daddr && iph->proto == qp->iph->proto)
		{	// 找到后重置计时器，在这删除，在ip_find外面新增一个计时
			del_timer(&qp->timer);	/* So it doesn't vanish on us. The timer will be reset anyway */
			//sti();
			return(qp);
		}
	}
	//sti();
	return(NULL);
}

struct sk_buff *ip_defrag(struct iphdr *iph, struct sk_buff *skb, struct device *dev)
{
	struct ipfrag *prev, *next;
	struct ipq *qp;
	int flags, offset;

	qp = ip_find(iph);// 根据ip头找是否已经存在分片队列
	offset = ntohs(iph->frag_offset);
	flags = offset & ~IP_OFFSET; // 取得三个分片标记位
	offset &= IP_OFFSET;//取得分片偏移

}
