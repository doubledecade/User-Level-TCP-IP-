//
// Created by MHP on 2021/12/14.
//

#ifndef USER_LEVEL_IP_UTILS_H
#define USER_LEVEL_IP_UTILS_H
#include <arpa/inet.h>
#define CMDBUFLEN 100
int run_cmd(char *cmd, ...);
uint32_t ip_parse(char *addr);
uint32_t sum_every_16bits(void *addr, int count);
uint16_t checksum(void *addr, int count, int start_sum);
#endif //USER_LEVEL_IP_UTILS_H
