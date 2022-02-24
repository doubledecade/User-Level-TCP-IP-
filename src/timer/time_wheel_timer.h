//
// Created by AT on 2022/2/18.
//

#ifndef USER_LEVEL_IP_TIME_WHEEL_TIMER_H
#define USER_LEVEL_IP_TIME_WHEEL_TIMER_H
#include <time.h>
#include <netinet/in.h>
#include <stdio.h>
#include "malloc.h"
#include "stdlib.h"
#define BUFFER_SIZE    64

struct tw_timer
{
	struct tw_timer *next;  //指向下一个定时器
	struct tw_timer *prev;  //指向前一个定时器
	unsigned long data;
	int rotation;  /*记录定时器在时间轮转多少圈后生效*/
	int time_slot; /*记录定时器属于时间轮上哪个槽(对应的链表，下同) */
	void (*cb_func) (unsigned long);  /*定时器回调函数*/
};

//时间轮上槽的数目
static const int N = 60;
//每1s时间转动一次，即槽间隔为1s
static const int SI = 1;
static int cur_slot = 0;
//时间轮的槽， 其中每个元素指向一个定时器链表， 链表无序
struct tw_timer *slots[N];



#endif //USER_LEVEL_IP_TIME_WHEEL_TIMER_H
