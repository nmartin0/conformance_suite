/*
 * memset: unsigned conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memset.002
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memset)(void *, int, size_t) = memset;

int main(void)
{
	unsigned char b[4];
	static const struct { int c; unsigned char want; } t[] = { { 0x1a5, 0xa5 }, { -1, 0xff }, { 256, 0 }, { 0x80, 0x80 }, { 0, 0 } };
	size_t k;
	int i;
	pc_begin("XSH.memset.002");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++) {
		for (i = 0; i < 4; i++) b[i] = 0x33;
		(void)f_memset(b, t[k].c, 4);
		for (i = 0; i < 4; i++)
			pc_check(b[i] == t[k].want, "value %d stored 0x%x, expected 0x%x", t[k].c, b[i], t[k].want);
	}
	pc_pass();
}
