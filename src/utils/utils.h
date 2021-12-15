//
// Created by MHP on 2021/12/14.
//

#ifndef USER_LEVEL_IP_UTILS_H
#define USER_LEVEL_IP_UTILS_H
#include <arpa/inet.h>
#define CMDBUFLEN 100
int run_cmd(char *cmd, ...);
uint32_t ip_parse(char *addr);
#endif //USER_LEVEL_IP_UTILS_H
