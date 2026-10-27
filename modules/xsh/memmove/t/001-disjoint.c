/*
 * memmove: disjoint.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmove.001
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmove)(void *, const void *, size_t) = memmove;

int main(void)
{
	static const unsigned char src[6] = { 0, 1, 0x80, 0xff, 0, 7 };
	unsigned char dst[10];
	int i;
	pc_begin("XSH.memmove.001");
	pc_test();
	memset(dst, 0xaa, sizeof dst);
	pc_check(f_memmove(dst + 2, src, 6) == dst + 2, "the return value was not the destination");
	for (i = 0; i < 10; i++) {
		if (i >= 2 && i < 8)
			pc_check(dst[i] == src[i - 2], "byte %d was not copied", i - 2);
		else
			pc_check(dst[i] == 0xaa, "byte %d outside the destination changed", i);
	}
	pc_pass();
}
