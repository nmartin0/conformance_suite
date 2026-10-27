/*
 * strlcpy: cat fits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.008
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[16];
	pc_begin("XSH.strlcpy.008");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab");
	pc_check(f_strlcat(d, "cd", 16) == 4, "the return value was not the total length");
	pc_check(memcmp(d, "abcd", 5) == 0, "the result is not \"abcd\" and a terminator");
	pc_pass();
}
