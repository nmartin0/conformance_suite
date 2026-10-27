/*
 * strncpy: stpncpy no null.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncpy.007
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncpy)(char *restrict, const char *restrict, size_t) = strncpy;
static char *(*volatile f_stpncpy)(char *restrict, const char *restrict, size_t) = stpncpy;

int main(void)
{
	char d[9];
	pc_begin("XSH.strncpy.007");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_stpncpy(d, "abcdef", 3) == d + 3, "n shorter than the string: wrong return value");
	pc_check(memcmp(d, "abcXXXXXX", 9) == 0, "n shorter than the string: wrong contents");
	memset(d, 'X', sizeof d);
	pc_check(f_stpncpy(d, "abc", 3) == d + 3, "n equal to the length: wrong return value");
	pc_check(memcmp(d, "abcXXXXXX", 9) == 0, "n equal to the length: a terminator was written");
	pc_pass();
}
