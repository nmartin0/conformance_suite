/*
 * memset: fill.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memset.001
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memset)(void *, int, size_t) = memset;

int main(void)
{
	unsigned char b[12];
	int i;
	pc_begin("XSH.memset.001");
	pc_test();
	for (i = 0; i < 12; i++) b[i] = 0xaa;
	pc_check(f_memset(b + 3, 0x5a, 5) == b + 3, "the return value was not the first argument");
	for (i = 0; i < 12; i++)
		pc_check(b[i] == ((i >= 3 && i < 8) ? 0x5a : 0xaa), "byte %d has the wrong value 0x%x", i, b[i]);
	pc_pass();
}
