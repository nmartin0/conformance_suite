/*
 * memcpy: return value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcpy.002
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memcpy)(void *restrict, const void *restrict, size_t) = memcpy;

int main(void)
{
	char src[4] = "abc", dst[4];
	pc_begin("XSH.memcpy.002");
	pc_test();
	pc_check(f_memcpy(dst, src, 4) == dst, "the return value was not the destination");
	pc_check(f_memcpy(dst + 1, src, 0) == dst + 1, "the return value for n 0 was not the destination");
	pc_pass();
}
