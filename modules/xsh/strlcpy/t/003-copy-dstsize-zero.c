/*
 * strlcpy: copy dstsize zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.003
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[4];
	pc_begin("XSH.strlcpy.003");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_strlcpy(d, "abcdef", 0) == 6, "the return value was not the length of the source");
	pc_check(memcmp(d, "XXXX", 4) == 0, "the destination changed");
	pc_pass();
}
