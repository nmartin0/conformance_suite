/*
 * strncpy: n less than length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncpy.002
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncpy)(char *restrict, const char *restrict, size_t) = strncpy;
static char *(*volatile f_stpncpy)(char *restrict, const char *restrict, size_t) = stpncpy;

int main(void)
{
	char d[9];
	pc_begin("XSH.strncpy.002");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_strncpy(d, "abcdef", 3) == d, "the return value was not the destination");
	pc_check(memcmp(d, "abcXXXXXX", 9) == 0, "only the first 3 bytes should have changed");
	pc_pass();
}
