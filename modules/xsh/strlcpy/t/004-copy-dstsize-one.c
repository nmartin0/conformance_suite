/*
 * strlcpy: copy dstsize one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.004
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[4];
	pc_begin("XSH.strlcpy.004");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_strlcpy(d, "abc", 1) == 3, "the return value was not the length of the source");
	pc_check(memcmp(d, "\0XXX", 4) == 0, "the destination is not a terminator and the old bytes");
	pc_pass();
}
