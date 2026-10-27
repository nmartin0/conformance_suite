/*
 * strlcpy: copy fits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.001
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[16];
	pc_begin("XSH.strlcpy.001");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_strlcpy(d, "abc", 16) == 3, "the return value was not the length of the source");
	pc_check(memcmp(d, "abc\0XXXXXXXXXXXX", 16) == 0, "the destination is not \"abc\" and a terminator");
	pc_pass();
}
