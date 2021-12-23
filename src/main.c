#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <tuntap.h>
#include <netdev.h>
//
// Created by MHP on 2021/12/23.
//
sigset_t mask;
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
}

int main(int argc,char **argv)
{
	init_signals();
	init_stack();
}