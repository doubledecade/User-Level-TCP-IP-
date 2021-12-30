#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <tuntap.h>
#include <netdev.h>
#include <pthread.h>
#include "route.h"
#include "netdev.h"
//
// Created by MHP on 2021/12/23.
//
static pthread_t threads[4];
sigset_t mask;
int running = 1;

#define THREAD_CORE 0
static void create_thread(pthread_t id, void *(*func) (void *))
{
    if (pthread_create(&threads[id], NULL,
                       func, NULL) != 0) {
        printf("Could not create core thread\n");
    }
}

static void init_signals()
{
	int err;
	//初始化信号
	sigemptyset(&mask);
	sigaddset(&mask,SIGINT);
	sigaddset(&mask,SIGQUIT);
	//只在主线程中接收信号
	if((err=pthread_sigmask(SIG_BLOCK,&mask,NULL))!=0)
	{
		printf("SIG_BLOCK error\n");
		exit(1);
	}
}
static void init_stack()
{
	tun_init("tap0","/dev/net/tap","10.0.0.5","10.0.0.0/24");
	netdev_init("10.0.0.4","00:0c:29:6d:50:25");
	route_init();
}

void run_threads() {
    create_thread(THREAD_CORE, netdev_rx_loop);
}
//等待回收线程
static void wait_for_threads()
{
    int i;
    for ( i = 0; i < 3; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Error when joining threads\n");
            exit(1);
        }
    }
}
int main(int argc, char **argv)
{
    printf("开始");
	init_signals();
	init_stack();
    run_threads();
    wait_for_threads();
}