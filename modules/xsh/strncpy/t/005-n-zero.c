/*
 * strncpy: n zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncpy.005
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncpy)(char *restrict, const char *restrict, size_t) = strncpy;
static char *(*volatile f_stpncpy)(char *restrict, const char *restrict, size_t) = stpncpy;

int main(void)
{
	char d[4] = "abc";
	pc_begin("XSH.strncpy.005");
	pc_test();
	pc_check(f_strncpy(d, "xyz", 0) == d, "strncpy() did not return the destination");
	pc_check(memcmp(d, "abc", 4) == 0, "strncpy() changed the destination");
	pc_check(f_stpncpy(d, "xyz", 0) == d, "stpncpy() did not return the destination");
	pc_check(memcmp(d, "abc", 4) == 0, "stpncpy() changed the destination");
	pc_pass();
}
