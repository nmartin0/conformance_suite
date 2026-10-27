/*
 * memcpy: n zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcpy.003
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memcpy)(void *restrict, const void *restrict, size_t) = memcpy;

int main(void)
{
	char src[4] = "xyz", dst[4] = "abc";
	pc_begin("XSH.memcpy.003");
	pc_test();
	(void)f_memcpy(dst, src, 0);
	pc_check(memcmp(dst, "abc", 4) == 0, "a copy of 0 bytes changed the destination");
	pc_pass();
}
