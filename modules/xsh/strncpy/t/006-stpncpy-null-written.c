/*
 * strncpy: stpncpy null written.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncpy.006
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncpy)(char *restrict, const char *restrict, size_t) = strncpy;
static char *(*volatile f_stpncpy)(char *restrict, const char *restrict, size_t) = stpncpy;

int main(void)
{
	char d[12];
	pc_begin("XSH.strncpy.006");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_stpncpy(d, "ab", 6) == d + 2, "the return value was not the first zero byte written");
	pc_check(memcmp(d, "ab\0\0\0\0XXXXXX", 12) == 0, "the destination is not \"ab\" padded with zero bytes to 6");
	memset(d, 'X', sizeof d);
	pc_check(f_stpncpy(d, "", 4) == d, "an empty source did not give the destination");
	pc_check(memcmp(d, "\0\0\0\0XXXXXXXX", 12) == 0, "an empty source did not give four zero bytes");
	pc_pass();
}
