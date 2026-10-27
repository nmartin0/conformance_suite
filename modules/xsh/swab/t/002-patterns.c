/*
 * swab: patterns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.swab.002
 */
#include "pc.h"
#include <string.h>
#include <unistd.h>
static void (*volatile f_swab)(const void *restrict, void *restrict, ssize_t) = swab;

int main(void)
{
	unsigned char s[6] = { 0x00, 0xff, 0x80, 0x7f, 0x01, 0xfe };
	static const unsigned char keep[6] = { 0x00, 0xff, 0x80, 0x7f, 0x01, 0xfe }, want[6] = { 0xff, 0x00, 0x7f, 0x80, 0xfe, 0x01 };
	unsigned char d[8];
	pc_begin("XSH.swab.002");
	pc_test();
	memset(d, 0x55, sizeof d);
	f_swab(s, d, 6);
	pc_check(memcmp(d, want, 6) == 0, "the bytes were not exchanged pairwise");
	pc_check(d[6] == 0x55 && d[7] == 0x55, "bytes beyond nbytes changed");
	pc_check(memcmp(s, keep, 6) == 0, "the source changed");
	pc_pass();
}
