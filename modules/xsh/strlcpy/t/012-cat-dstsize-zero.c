/*
 * strlcpy: cat dstsize zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.012
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[4];
	pc_begin("XSH.strlcpy.012");
	pc_test();
	strcpy(d, "ab");
	pc_check(f_strlcat(d, "xyz", 0) == 3, "the return value was not the length of the source");
	pc_check(memcmp(d, "ab", 3) == 0, "the destination changed");
	pc_pass();
}
