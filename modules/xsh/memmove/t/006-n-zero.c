/*
 * memmove: n zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmove.006
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmove)(void *, const void *, size_t) = memmove;

int main(void)
{
	char src[4] = "xyz", dst[4] = "abc";
	pc_begin("XSH.memmove.006");
	pc_test();
	pc_check(f_memmove(dst, src, 0) == dst, "the return value was not the destination");
	pc_check(memcmp(dst, "abc", 4) == 0, "a move of 0 bytes changed the destination");
	pc_pass();
}
