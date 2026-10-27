/*
 * strlcpy: cat empty operands.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.013
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[8];
	pc_begin("XSH.strlcpy.013");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab");
	pc_check(f_strlcat(d, "", 8) == 2, "empty source: wrong return value");
	pc_check(memcmp(d, "ab\0XXXXX", 8) == 0, "empty source: the destination changed");
	memset(d, 'X', sizeof d); d[0] = 0;
	pc_check(f_strlcat(d, "xyz", 8) == 3, "empty destination: wrong return value");
	pc_check(memcmp(d, "xyz\0XXXX", 8) == 0, "empty destination: wrong result");
	pc_pass();
}
