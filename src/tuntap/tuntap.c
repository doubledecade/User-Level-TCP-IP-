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
static char *tap_name;


static int set_route(char *dev_t,const char *cidr)
{
	return run_cmd("ip route add dev %s %s", dev_t, cidr);
}

static int set_address(char *dev_t,const char *cidr)
{
    return run_cmd("ip address add dev %s local %s",dev_t,cidr);
}

static  int set_up(char *dev_t)
{
    return run_cmd("ip link set dev %s up",dev_t);
}
static int tun_alloc(char * dev_t,const char * tap_path)
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
    if(*dev_t)
    {
    	strncpy(ifr.ifr_name,dev_t,IFNAMSIZ);
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
void  tun_init( char *dev_name,const char * tap_path,const char *tapaddr,const char *taproute)
{
       tap_name= (char *)malloc(strlen(dev_name)+1);
        strcpy(tap_name, dev_name);
       tun_fd=tun_alloc(tap_name,tap_path);
       if(set_up(tap_name)!=0)
       {
           perror("err when set up");
       }
       if(set_route(tap_name,taproute)!=0)
       {
           perror("err set route");
       }
       if(set_address(tap_name,tapaddr))
       {
           perror("err set addr");
       }


}

void free_tun()
{
    if(tap_name==NULL)
    {
        return;
    }
    free(tap_name);
}

int  get_tun_fd( char *dev_name,const char * tap_path,const char *tapaddr,const char *taproute)
{
	tap_name= (char *)malloc(strlen(dev_name)+1);
	strcpy(tap_name, dev_name);
	tun_fd=tun_alloc(tap_name,tap_path);
	if(set_up(tap_name)!=0)
	{
		perror("err when set up");
	}
	if(set_route(tap_name,taproute)!=0)
	{
		perror("err set route");
	}
	if(set_address(tap_name,tapaddr))
	{
		perror("err set addr");
	}

	return tun_fd;
}

int tun_read_fd(char *buf,int len,int tun_fd_out)
{
	if(tun_fd==-1)
	{
		perror("ERR: tun_fd not initialized");
		return -1;
	}
	return read(tun_fd_out,buf,len);
}