/*
 * memcpy: copy exact.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcpy.001
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memcpy)(void *restrict, const void *restrict, size_t) = memcpy;

int main(void)
{
	static const unsigned char src[8] = { 0, 1, 0x80, 0xff, 0, 7, 0x7f, 0x55 };
	unsigned char dst[16];
	int i;
	pc_begin("XSH.memcpy.001");
	pc_test();
	memset(dst, 0xaa, sizeof dst);
	(void)f_memcpy(dst + 4, src, 8);
	for (i = 0; i < 16; i++) {
		if (i >= 4 && i < 12)
			pc_check(dst[i] == src[i - 4], "byte %d was not copied", i - 4);
		else
			pc_check(dst[i] == 0xaa, "byte %d outside the destination changed", i);
	}
	pc_pass();
}
