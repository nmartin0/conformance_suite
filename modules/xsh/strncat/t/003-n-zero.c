/*
 * strncat: n zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncat.003
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncat)(char *restrict, const char *restrict, size_t) = strncat;

int main(void)
{
	char d[8];
	pc_begin("XSH.strncat.003");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab");
	pc_check(f_strncat(d, "cdef", 0) == d, "the return value was not the destination");
	pc_check(memcmp(d, "ab", 3) == 0, "the destination changed or lost its terminator");
	pc_pass();
}
