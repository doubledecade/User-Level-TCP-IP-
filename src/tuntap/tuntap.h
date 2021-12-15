//
// Created by MHP on 2021/12/13.
//

#ifndef USER_LEVEL_IP_TUNTAP_H
#define USER_LEVEL_IP_TUNTAP_H
void  tun_init( char *dev_name,const char * tap_path,const char *tapaddr,const char *taproute);
void free_tun();
int tun_write(char *buf,int len);
int tun_read(char *buf,int len);

#endif //USER_LEVEL_IP_TUNTAP_H
