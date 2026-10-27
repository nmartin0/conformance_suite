/*
 * strncpy: after null not copied.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncpy.004
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncpy)(char *restrict, const char *restrict, size_t) = strncpy;
static char *(*volatile f_stpncpy)(char *restrict, const char *restrict, size_t) = stpncpy;

int main(void)
{
	static const char s[5] = { 'a', 'b', 0, 'c', 'd' };
	char d[7];
	pc_begin("XSH.strncpy.004");
	pc_test();
	memset(d, 'X', sizeof d);
	(void)f_strncpy(d, s, 5);
	pc_check(memcmp(d, "ab\0\0\0XX", 7) == 0, "bytes after the zero byte were copied or padding is wrong");
	pc_pass();
}
