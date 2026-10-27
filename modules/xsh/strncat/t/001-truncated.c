/*
 * strncat: truncated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncat.001
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncat)(char *restrict, const char *restrict, size_t) = strncat;

int main(void)
{
	char d[12];
	pc_begin("XSH.strncat.001");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab");
	d[3] = 'X'; d[4] = 'X'; d[5] = 'X';
	pc_check(f_strncat(d, "cdef", 2) == d, "the return value was not the destination");
	pc_check(memcmp(d, "abcd", 5) == 0, "the result is not \"abcd\" followed by a terminator");
	pc_pass();
}
