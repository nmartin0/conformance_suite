/*
 * swab: large.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.swab.003
 */
#include "pc.h"
#include <string.h>
#include <unistd.h>
static void (*volatile f_swab)(const void *restrict, void *restrict, ssize_t) = swab;

int main(void)
{
	unsigned char s[1010], d[1020];
	int so, d_o, i;
	pc_begin("XSH.swab.003");
	for (i = 0; i < (int)sizeof s; i++) s[i] = (unsigned char)(i * 13 + 5);
	pc_test();
	for (so = 0; so < 4; so++)
		for (d_o = 0; d_o < 4; d_o++) {
			memset(d, 0xaa, sizeof d);
			f_swab(s + so, d + 8 + d_o, 1000);
			for (i = 0; i < (int)sizeof d; i++) {
				unsigned char want;
				if (i >= 8 + d_o && i < 8 + d_o + 1000) {
					int j = i - 8 - d_o;
					want = s[so + (j ^ 1)];
				} else
					want = 0xaa;
				if (d[i] != want)
					pc_fail("source offset %d, destination offset %d: byte %d is wrong", so, d_o, i);
			}
		}
	pc_pass();
}
