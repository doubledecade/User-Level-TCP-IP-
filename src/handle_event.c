#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include  <signal.h>
#include "tuntap.h"
#include "netdev.h"
#define  MAX_EVENT_NUMBER   1024
#define  BUFFER_SIZE        10
#define  TIMESLOT    5
static int pipefd[2];
bool stop_server = false;
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


/*信号处理函数*/
void sig_handler(int sig)
{
	/* 保留原来的errno, 在函数最后恢复，以保证函数的可重入性
	 * */
	printf("获取型号\n");
	int save_errno = errno;
	int msg = sig;
	send(pipefd[1], (char *)&msg, 1, 0); /*将信号值写入管道，以通知主循环*/
	errno = save_errno;
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
			netdev_rx(sockfd);
		}
	}
}

/*ET模式的工作流程*/
void et(struct epoll_event *events, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE];
	int i;
	for( i = 0; i < number; i++)
	{
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd)
		{
			struct sockaddr_in client_address;
			socklen_t client_addrlength = sizeof(client_address);
			int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
			addfd(epollfd, connfd, true); /*connfd 开启ET 模式*/
		}
		else if (events[i].events & EPOLLIN)
		{
			/*这段代码不会重复触发，所以我们循环读取数据，
			 * 以确保把socket读缓存中的所有数据读出
			 */
			fprintf(stdout, "event trigger once: EPOLLIN\n");
			while(1)
			{
				memset(buf, '\0', BUFFER_SIZE);
				int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
				if(ret < 0)
				{
					/*对于非阻塞IO，下的条件成立表示数据已经全部读取完毕。此后，
					 * epoll就能再次触发sockfd上的EPOLLIN事件，以驱动下一次读操作
					 */
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK) )
					{
						fprintf(stdout, "read later\n");
						break;
					}
					close(sockfd);
					break;
				}
				else if (ret == 0)
				{
					close(sockfd);
				}
				else
				{
					fprintf(stdout, "got %d bytes of content: %s\n", ret, buf);
				}
			}
		}
		else
		{
			fprintf(stdout, "something else happened\n");
		}
	}
}

int main()
{


	int ret = 0;
	struct epoll_event events[MAX_EVENT_NUMBER];
	int epollfd = epoll_create(5);
	assert(epollfd != -1);
	/*使用socketpair创建管道， 注册pipefd[0]上的可读事件*/
	ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
	assert( ret != -1);
	setnonblocking(pipefd[1]);
	addfd(epollfd, pipefd[0],false);
	int tun_fd=get_tun_fd("tap0","/dev/net/tap","10.0.0.5","10.0.0.0/24");
	assert(tun_fd != -1);
	addfd(epollfd, tun_fd,false);
	/*设置一些信号的处理函数*/
	addsig(SIGHUP);
	addsig(SIGCHLD);
	addsig(SIGTERM);
	addsig(SIGINT);
	addsig(SIGPIPE);
	//超时型号
	addsig(SIGALRM);
	alarm(TIMESLOT);



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