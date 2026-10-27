/*
 * htons() gives the value with its most significant byte first in memory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.htonl.001
 */
#include "pc.h"
#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>

int main(void)
{
	volatile uint16_t in1 = 0x0102, in2 = 0xfffe;
	uint16_t a, b;
	unsigned char ba[2], bb[2];

	pc_begin("XSH.htonl.001");
	pc_test();
	a = htons(in1);
	b = htons(in2);
	memcpy(ba, &a, 2);
	memcpy(bb, &b, 2);
	pc_check(ba[0] == 0x01 && ba[1] == 0x02, "htons(0x0102) has bytes %02x %02x", ba[0], ba[1]);
	pc_check(bb[0] == 0xff && bb[1] == 0xfe, "htons(0xfffe) has bytes %02x %02x", bb[0], bb[1]);
	pc_pass();
}
