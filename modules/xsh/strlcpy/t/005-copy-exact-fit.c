/*
 * strlcpy: copy exact fit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.005
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[8];
	pc_begin("XSH.strlcpy.005");
	pc_test();
	memset(d, 'X', sizeof d);
	pc_check(f_strlcpy(d, "abc", 4) == 3, "dstsize 4: wrong return value");
	pc_check(memcmp(d, "abc\0XXXX", 8) == 0, "dstsize 4: wrong destination");
	memset(d, 'X', sizeof d);
	pc_check(f_strlcpy(d, "abc", 3) == 3, "dstsize 3: wrong return value");
	pc_check(memcmp(d, "ab\0XXXXX", 8) == 0, "dstsize 3: wrong destination");
	pc_pass();
}
