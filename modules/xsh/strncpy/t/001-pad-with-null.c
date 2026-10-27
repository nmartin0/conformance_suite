/*
 * strncpy: pad with null.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncpy.001
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncpy)(char *restrict, const char *restrict, size_t) = strncpy;
static char *(*volatile f_stpncpy)(char *restrict, const char *restrict, size_t) = stpncpy;

int main(void)
{
	char d[12];
	pc_begin("XSH.strncpy.001");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_strncpy(d, "ab", 6) == d, "the return value was not the destination");
	pc_check(memcmp(d, "ab\0\0\0\0XXXXXX", 12) == 0, "the destination is not \"ab\" padded with zero bytes to 6");
	pc_pass();
}
