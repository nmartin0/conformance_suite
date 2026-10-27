/*
 * strlcpy: cat nul stops.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.014
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	static const char s[3] = { 'c', 0, 'd' };
	char d[8];
	pc_begin("XSH.strlcpy.014");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab");
	d[3] = 'X';
	pc_check(f_strlcat(d, s, 8) == 3, "wrong return value");
	pc_check(memcmp(d, "abc\0XXXX", 8) == 0, "bytes after the source terminator were appended");
	pc_pass();
}
