/*
 * strncpy: n equals length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncpy.003
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncpy)(char *restrict, const char *restrict, size_t) = strncpy;
static char *(*volatile f_stpncpy)(char *restrict, const char *restrict, size_t) = stpncpy;

int main(void)
{
	char d[8];
	pc_begin("XSH.strncpy.003");
	pc_test();
	memset(d, 'X', sizeof d);
	(void)f_strncpy(d, "abc", 3);
	pc_check(memcmp(d, "abcXXXXX", 8) == 0, "a terminator was written or the copy is wrong");
	pc_pass();
}
