/*
 * ntohs() and ntohl() read a value stored most significant byte first.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.htonl.003
 */
#include "pc.h"
#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>

int main(void)
{
	unsigned char s[2] = { 0x01, 0x02 }, l[4] = { 0x01, 0x02, 0x03, 0x04 };
	uint16_t sv;
	uint32_t lv;

	pc_begin("XSH.htonl.003");
	memcpy(&sv, s, 2);
	memcpy(&lv, l, 4);
	pc_test();
	pc_check(ntohs(sv) == 0x0102, "ntohs gave %#x, expected 0x102", (unsigned)ntohs(sv));
	pc_check(ntohl(lv) == 0x01020304UL, "ntohl gave %#lx, expected 0x1020304", (unsigned long)ntohl(lv));
	pc_pass();
}
