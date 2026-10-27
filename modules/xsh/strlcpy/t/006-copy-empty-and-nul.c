/*
 * strlcpy: copy empty and nul.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.006
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	static const char s[5] = { 'a', 'b', 0, 'c', 'd' };
	char d[8];
	pc_begin("XSH.strlcpy.006");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_strlcpy(d, "", 8) == 0, "an empty source: wrong return value");
	pc_check(memcmp(d, "\0XXXXXXX", 8) == 0, "an empty source: wrong destination");
	memset(d, 'X', sizeof d);
	pc_check(f_strlcpy(d, s, 8) == 2, "a source with trailing bytes: wrong return value");
	pc_check(memcmp(d, "ab\0XXXXX", 8) == 0, "bytes after the source terminator were copied");
	pc_pass();
}
