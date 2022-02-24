#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <tuntap.h>
#include <netdev.h>
#include <pthread.h>
#include "route.h"
#include "netdev.h"
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <memory.h>
#include <assert.h>
#include <unistd.h>
#include "stdbool.h"
//
// Created by MHP on 2021/12/23.
//

#define THREAD_CORE 0
#define  MAX_EVENT_NUMBER   1024
#define TIMESLOT 5
#define BUFFER_SIZE 10
static int pipefd[2];//信号处理队列
static pthread_t threads[4];
sigset_t mask;
bool stop_server = false;
int running = 1;

void addsig(int sig);

static void create_thread(pthread_t id, void *(*func) (void *))
{
    if (pthread_create(&threads[id], NULL,
                       func, NULL) != 0) {
        printf("Could not create core thread\n");
    }
}

static void init_signals()
{

	addsig(SIGHUP);
	addsig(SIGCHLD);
	addsig(SIGTERM);
	addsig(SIGINT);
	addsig(SIGPIPE);
	//超时型号
	addsig(SIGALRM);
	//开启计时器;
	alarm(TIMESLOT);
}
static void init_stack()
{
//	tun_init("tap0","/dev/net/tap","10.0.0.5","10.0.0.0/24");
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

/*信号处理函数*/
void sig_handler(int sig)
{
	/* 保留原来的errno, 在函数最后恢复，以保证函数的可重入性
	 * */

	int save_errno = errno;
	int msg = sig;
	send(pipefd[1], (char *)&msg, 1, 0); /*将信号值写入管道，以通知主循环*/
	errno = save_errno;
}

/*将文件描述符设置成非阻塞的 */
int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

/* 将文件描述符fd上的EPOLLIN注册到epollfd指示的epoll内核事件表中，参数enable_et
 * 指定是否对fd启用ET模式
 * */
void addfd(int epollfd, int fd, bool enable_et)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	if (enable_et)
	{
		event.events |= EPOLLET;
	}

	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}
/* 设置信号处理函数
 * */
void addsig(int sig)
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = sig_handler;
	sa.sa_flags   = SA_RESTART;
	sigfillset(&sa.sa_mask);
	assert(sigaction(sig, &sa, NULL) != -1);
}

/*LT模式的工作流程 */
void lt( struct epoll_event *events, int number, int tunfd)
{
	char buf[BUFFER_SIZE];
	int i;
	for( i = 0; i < number; i++)
	{
		int sockfd = events[i].data.fd;
		printf("%d->有数据\n",sockfd);
		if ((sockfd == pipefd[0]) && (events[i].events & EPOLLIN))
		{
			char signals[1024];
			int ret = recv(pipefd[0], signals, sizeof(signals), 0);
			if (ret == -1)
			{
				continue;
			}
			else if (ret == 0)
			{
				continue;
			}
			else
			{
				int j;
				for(j=0;j<ret;j++)
				{
					switch (signals[j])
					{
						case SIGCHLD:
						case SIGHUP:
						{
							continue;
						}
						case SIGTERM:
						case SIGINT:
						{
							stop_server = true;
							printf("程序关闭\n");

						}
							break;
							//超时信号
						case SIGALRM:
						{
							printf("时间到\n");
							alarm(TIMESLOT);
						}
							break;
						default:
							break;
					}
				}
			}
		}
		else if(events[i].events & EPOLLIN)
		{
			printf("有数据\n");
			netdev_rx(sockfd);
		}
	}
}

int init_run_epoll()
{
	struct epoll_event events[MAX_EVENT_NUMBER];
	int epollfd = epoll_create(5);
	assert(epollfd != -1);
	int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
	assert( ret != -1);
	setnonblocking(pipefd[1]);
	addfd(epollfd, pipefd[0],false);
	int tun_fd=get_tun_fd("tap0","/dev/net/tap","10.0.0.5","10.0.0.0/24");
	printf("%d tun_fd\n",tun_fd);
	assert(tun_fd != -1);
	addfd(epollfd, tun_fd,false);

	while(!stop_server)
	{
		int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
		printf("%d\n",number);
		if ((number < 0) && (errno != EINTR))
		{
			fprintf(stderr, "epoll failure %s\n",strerror(errno));
			break;
		}

		lt(events, number, tun_fd);  /*使用LT 模式*/
		//et(events, ret, epollfd, listenfd);  /*使用ET模式 */
	}


	return 0;
}

int main(int argc, char **argv)
{
	init_signals();
	init_stack();
	init_run_epoll();
	return 0;
}