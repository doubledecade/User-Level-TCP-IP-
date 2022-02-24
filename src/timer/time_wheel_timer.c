//
// Created by AT on 2022/2/18.
//

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>
#include "time_wheel_timer.h"

void InitWheel()
{
	for(int i = 0; i < N; i++)
	{
		slots[i] = NULL;   //初始化每个槽的头结点
	}
}

static struct tw_timer * CreatTwTimer(int rotation,int ts){
	struct tw_timer * timer =(struct tw_timer *)malloc(sizeof (struct tw_timer));
	if(timer==NULL)
	{
		printf("tw_timer结构体空间申请失败");
		exit(0);
	}
	timer->next=NULL;
	timer->prev=NULL;
	timer->rotation=rotation;
	timer->time_slot=ts;
	return timer;
}

struct tw_timer * add_timer(int timeout)
{
	if(timeout < 0)
	{
		return NULL;
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

	//计算待插入的定时器在时间轮转动多少圈后被触发
	int rotation = ticks / N;
	//计算待插入的定时器应该被插入哪个槽中
	int ts = (cur_slot + (ticks % N)) % N;

	struct tw_timer * timer=CreatTwTimer(rotation,ts);
	//如果第ts个槽中尚无任何定时器，则把新建的定时器插入其中，
	//并将该定时器设置为该槽的头结点
	if(!slots[ts])
	{
		fprintf(stdout, "add timer, rotation is %d, ts is %d, cur_slot is %d\n",
		        rotation, ts, cur_slot);
		slots[ts]=timer;
	}
	else
	{
		//头插法，讲新节点插入该槽头节点
		timer->next = slots[ts];
		slots[ts]->prev=timer;
		slots[ts] = timer;
	}
	return timer;
}

static void DestroyWheel()
{
	//遍历每个槽，并销毁其中的定时器
	for(int i = 0; i < N; i++)
	{
		struct tw_timer *tmp = slots[i];
		while(tmp)
		{
			slots[i] = tmp->next;
			//free(tmp);
			tmp = slots[i];
		}
	}
}

void DelTimer(struct tw_timer* timer)
{
	if(!timer)
	{
		return;
	}
	int ts = timer->time_slot;
	// slots[ts]是目标定时器所在槽的头结点。如果目标定时器就是该头结点，
	// 则需要重置第ts个槽的头结点
	if(timer == slots[ts])
	{
		slots[ts] = slots[ts]->next;
		if(slots[ts])
		{
			slots[ts]->prev=NULL;
			//free(timer);
		}
	}
	else
	{
		timer->prev->next=timer->next;
		if(timer->next)
		{
			timer->next->prev=timer->prev;
		}
		//free(timer);
	}
}

//SI 时间到后，调用该函数，时间轮向前滚动一个槽的间隔
void tick()
{
	struct tw_timer *tmp = slots[cur_slot]; //取得时间轮当前槽的头节点
	fprintf(stdout,"current slot is %d\n",cur_slot);
	while (tmp)
	{
		fprintf(stdout, "tick the timer once\n");
		//如果定时器的rotation值大于0, 则它在这一轮不起作用
		if(tmp->rotation > 0)
		{
			tmp->rotation--;
			tmp=tmp->next;
		}
			//否则， 说明定时器已到期，于是执行定时任务，然后删除该定时器
		else
		{
			//执行回调函数
			tmp->cb_func(tmp->data);
			//如果是头节点,替换下一个节点为头节点
			if(tmp == slots[cur_slot])
			{
				fprintf(stdout, "delete header in cur_slot\n");
				slots[cur_slot] = tmp->next;
				//free(tmp);
				if (slots[cur_slot])
				{
					slots[cur_slot]->prev = NULL;
				}
				tmp = slots[cur_slot];
			}
			else
			{
				tmp->prev->next = tmp->next;
				if(tmp->next)
				{
					tmp->next->prev = tmp->prev;
				}
				struct tw_timer *tmp2 = tmp->next;
				//free(tmp);
				tmp =tmp2;
			}

		}
	}
	cur_slot = ++ cur_slot % N; //更新时间轮的当前槽，以反应时间轮的转动
}