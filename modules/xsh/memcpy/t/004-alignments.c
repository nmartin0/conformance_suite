/*
 * memcpy: alignments.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcpy.004
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memcpy)(void *restrict, const void *restrict, size_t) = memcpy;

static unsigned char sbuf[512], dbuf[512];

int main(void)
{
	static const size_t sizes[] = { 1, 3, 7, 8, 9, 15, 16, 17, 31, 33, 257 };
	size_t k, i;
	int so, d_o;
	pc_begin("XSH.memcpy.004");
	for (i = 0; i < sizeof sbuf; i++)
		sbuf[i] = (unsigned char)(i * 7 + 3);
	pc_test();
	for (k = 0; k < sizeof sizes / sizeof sizes[0]; k++)
		for (so = 0; so < 8; so++)
			for (d_o = 0; d_o < 8; d_o++) {
				size_t n = sizes[k];
				memset(dbuf, 0xaa, sizeof dbuf);
				(void)f_memcpy(dbuf + 16 + d_o, sbuf + so, n);
				for (i = 0; i < sizeof dbuf; i++) {
					unsigned char want = (i >= 16 + (size_t)d_o && i < 16 + (size_t)d_o + n) ? sbuf[so + (i - 16 - (size_t)d_o)] : 0xaa;
					if (dbuf[i] != want)
						pc_fail("n %lu, src offset %d, dst offset %d: byte %lu is wrong", (unsigned long)n, so, d_o, (unsigned long)i);
				}
			}
	pc_pass();
}
