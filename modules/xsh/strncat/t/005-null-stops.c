/*
 * strncat: null stops.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncat.005
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncat)(char *restrict, const char *restrict, size_t) = strncat;

int main(void)
{
	static const char s[3] = { 'c', 0, 'd' };
	char d[10];
	pc_begin("XSH.strncat.005");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab");
	(void)f_strncat(d, s, 3);
	pc_check(memcmp(d, "abc", 4) == 0, "the bytes after the zero byte were appended");
	pc_pass();
}
