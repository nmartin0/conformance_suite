/*
 * strlcpy: copy truncated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.002
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[10];
	pc_begin("XSH.strlcpy.002");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_strlcpy(d, "abcdef", 4) == 6, "the return value was not the length of the source");
	pc_check(memcmp(d, "abc\0XXXXXX", 10) == 0, "the destination is not \"abc\", a terminator and the old bytes");
	pc_pass();
}
