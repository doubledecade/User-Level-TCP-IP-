//
// Created by MHP on 2021/12/13.
//
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include "tuntap.h"
#include "net/if.h"

static int tun_fd;
static char* dev;

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
}

void  tun_init(const char * tap_path)
{
       dev = calloc(10,1);
       tun_fd=tun_alloc(dev,tap_path);
}