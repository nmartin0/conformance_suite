/*
 * memset: alignments.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memset.004
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memset)(void *, int, size_t) = memset;

static unsigned char buf[512];

int main(void)
{
	static const size_t sizes[] = { 1, 3, 7, 8, 9, 15, 16, 17, 31, 33, 257 };
	size_t k, i;
	int o;
	pc_begin("XSH.memset.004");
	pc_test();
	for (k = 0; k < sizeof sizes / sizeof sizes[0]; k++)
		for (o = 0; o < 8; o++) {
			size_t n = sizes[k];
			for (i = 0; i < sizeof buf; i++) buf[i] = 0xaa;
			(void)f_memset(buf + 16 + o, 0x3c, n);
			for (i = 0; i < sizeof buf; i++) {
				unsigned char want = (i >= 16 + (size_t)o && i < 16 + (size_t)o + n) ? 0x3c : 0xaa;
				if (buf[i] != want)
					pc_fail("n %lu, offset %d: byte %lu is wrong", (unsigned long)n, o, (unsigned long)i);
			}
		}
	pc_pass();
}
