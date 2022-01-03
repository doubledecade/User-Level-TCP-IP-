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

uint32_t sum_every_16bits(void *addr, int count)
{
	register uint32_t sum = 0;
	uint16_t * ptr = addr;

	while( count > 1 )  {
		/*  This is the inner loop */
		sum += * ptr++;
		count -= 2;
	}

	/*  Add left-over byte, if any */
	if( count > 0 )
		sum += * (uint8_t *) ptr;

	return sum;
}

uint16_t checksum(void *addr, int count, int start_sum)
{
	/* Compute Internet Checksum for "count" bytes
	 *         beginning at location "addr".
	 * Taken from https://tools.ietf.org/html/rfc1071
	 */
	uint32_t sum = start_sum;

	sum += sum_every_16bits(addr, count);

	/*  Fold 32-bit sum to 16 bits */
	while (sum>>16)
		sum = (sum & 0xffff) + (sum >> 16);

	return ~sum;
}