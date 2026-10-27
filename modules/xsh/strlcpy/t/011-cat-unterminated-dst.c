/*
 * strlcpy: cat unterminated dst.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.011
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[8];
	pc_begin("XSH.strlcpy.011");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_strlcat(d, "xyz", 4) == 7, "the return value was not dstsize plus the length of the source");
	pc_pass();
}
