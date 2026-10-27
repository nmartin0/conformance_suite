/*
 * strncat: n equals length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncat.004
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncat)(char *restrict, const char *restrict, size_t) = strncat;

int main(void)
{
	char d[10];
	pc_begin("XSH.strncat.004");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab"); d[3] = 'X';
	(void)f_strncat(d, "cd", 2);
	pc_check(memcmp(d, "abcd", 5) == 0, "the result is not \"abcd\" followed by a terminator");
	pc_pass();
}
