//
// Created by MHP on 2021/12/13.
//
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include "tuntap.h"
#include "net/if.h"
#include "utils.h"

static int tun_fd=-1;
static char* dev;

static int set_route(char *dev,char *cidr)
{
	return run_cmd("ip route add dev %s %s", dev, cidr);
}
static int tun_alloc(char * dev,const char * tap_path)
{
    struct ifreq ifr;
    int fd,err;
    if((fd=open(tap_path,O_RDWR))<0)
    {
        perror("Cannot open TUN/TAP dev\n"
               "Make sure one exists with "
               "'$ mknod /dev/net/tap c 10 200'");
        exit(1);
    }
    memset(&ifr,0,sizeof(struct ifreq));
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if(*dev)
    {
    	strncpy(ifr.ifr_name,dev,IFNAMSIZ);
    }
    //ioctl是设备驱动程序中对设备的I/O通道进行管理的函数
    if((err =ioctl(fd,TUNSETIFF,(void *)&ifr)) <0)
    {
    	perror("ERR: Could not ioctl tun");
	    close(fd);
	    return err;
    }

	return fd;
}

int tun_read(char *buf,int len)
{
	if(tun_fd==-1)
	{
		perror("ERR: tun_fd not initialized");
		return -1;
	}
	return read(tun_fd,buf,len);
}
int tun_write(char *buf,int len)
{
	if(tun_fd==-1)
	{
		perror("ERR: tun_fd not initialized");
		return -1;
	}
	return write(tun_fd,buf,len);
}
void  tun_init(const char * tap_path)
{
       dev = calloc(10,1);
       tun_fd=tun_alloc(dev,tap_path);
}