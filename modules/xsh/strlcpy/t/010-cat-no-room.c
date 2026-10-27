/*
 * strlcpy: cat no room.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.010
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	char d[8];
	pc_begin("XSH.strlcpy.010");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab");
	pc_check(f_strlcat(d, "xyz", 3) == 5, "dstsize 3: wrong return value");
	pc_check(memcmp(d, "ab\0XXXXX", 8) == 0, "dstsize 3: the destination changed");
	pc_check(f_strlcat(d, "xyz", 2) == 5, "dstsize 2: wrong return value");
	pc_check(d[0] == 'a' && d[1] == 'b', "dstsize 2: the destination changed");
	pc_pass();
}
