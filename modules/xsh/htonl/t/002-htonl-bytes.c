/*
 * htonl() gives the value with its most significant byte first in memory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.htonl.002
 */
#include "pc.h"
#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>

int main(void)
{
	volatile uint32_t in1 = 0x01020304UL, in2 = 0xa1b2c3d4UL;
	uint32_t a, b;
	unsigned char ba[4], bb[4];

	pc_begin("XSH.htonl.002");
	pc_test();
	a = htonl(in1);
	b = htonl(in2);
	memcpy(ba, &a, 4);
	memcpy(bb, &b, 4);
	pc_check(ba[0] == 0x01 && ba[1] == 0x02 && ba[2] == 0x03 && ba[3] == 0x04, "htonl(0x01020304) has bytes %02x %02x %02x %02x", ba[0], ba[1], ba[2], ba[3]);
	pc_check(bb[0] == 0xa1 && bb[1] == 0xb2 && bb[2] == 0xc3 && bb[3] == 0xd4, "htonl(0xa1b2c3d4) has bytes %02x %02x %02x %02x", bb[0], bb[1], bb[2], bb[3]);
	pc_pass();
}
