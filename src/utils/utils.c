//
// Created by AT on 2021/12/14.
//

#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


int run_cmd(char *cmd,...)
{
	va_list ap;
	char buf[CMDBUFLEN];
	va_start(ap, cmd);
	vsnprintf(buf, CMDBUFLEN, cmd, ap);

	va_end(ap);
	return system(buf);
}

uint32_t ip_parse(char *addr)
{
	uint32_t dst = 0;

	if (inet_pton(AF_INET, addr, &dst) != 1) {
		perror("ERR: Parsing inet address failed");
		exit(1);
	}

	return ntohl(dst);
}
