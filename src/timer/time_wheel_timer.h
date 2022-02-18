//
// Created by AT on 2022/2/18.
//

#ifndef USER_LEVEL_IP_TIME_WHEEL_TIMER_H
#define USER_LEVEL_IP_TIME_WHEEL_TIMER_H
#include <time.h>
#include <netinet/in.h>
#include <stdio.h>

#define BUFFER_SIZE    64

struct tw_timer
{
	tw_timer *next;  //指向下一个定时器
	tw_timer *prev;  //指向前一个定时器
	int rotation;  /*记录定时器在时间轮转多少圈后生效*/
	int time_slot; /*记录定时器属于时间轮上哪个槽(对应的链表，下同) */
	void (*cb_func) (unsigned long);  /*定时器回调函数*/
};

//时间轮上槽的数目
static const int N = 60;
//每1s时间转动一次，即槽间隔为1s
static const int SI = 1;

//时间轮的槽， 其中每个元素指向一个定时器链表， 链表无序
tw_timer *slots[N];

void init_wheel()
{
	for(int i = 0; i < N; i++)
	{
		slots[i] = NULL;   //初始化每个槽的头结点
	}
}

static void DestroyWheel()
{
	//遍历每个槽，并销毁其中的定时器
	for(int i = 0; i < N; i++)
	{
		tw_timer *tmp = slots[i];
		while(tmp)
		{
			slots[i] = tmp->next;
			delete tmp;
			tmp = slots[i];
		}
	}
}

void add_timer(int timeout)
{
	if(timeout < 0)
	{
		return;
	}
	int ticks=0;
	/*下面根据待插入定时器的超时值计算它将在时间轮转动多少个滴答后被触发，
	 * 并将该滴答数存储于变量ticks中。如果待插入定时器的超时值小于时间轮的槽间隔SI，
	 * 则将ticks向上折合为1,否则就将ticks向下折合为timeout/SI
	 */

	if(timeout < SI)
	{
		ticks = 1;
	}
	else
	{
		ticks = timeout / SI;
	}


}

#endif //USER_LEVEL_IP_TIME_WHEEL_TIMER_H
