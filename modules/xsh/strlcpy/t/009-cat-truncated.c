/*
 * strlcpy: cat truncated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.009
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[10];
	pc_begin("XSH.strlcpy.009");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab"); d[3] = 'X';
	pc_check(f_strlcat(d, "cdefg", 5) == 7, "the return value was not the length it tried to create");
	pc_check(memcmp(d, "abcd\0XXXXX", 10) == 0, "the destination is not \"abcd\", a terminator and the old bytes");
	pc_pass();
}
