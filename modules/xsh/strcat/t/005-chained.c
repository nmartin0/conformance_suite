/*
 * strcat: chained.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcat.005
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strcat)(char *restrict, const char *restrict) = strcat;

int main(void)
{
	char d[16];
	pc_begin("XSH.strcat.005");
	pc_test();
	d[0] = 0;
	pc_check(f_strcat(f_strcat(f_strcat(d, "a"), "bc"), "def") == d, "the chained result was not the destination");
	pc_check(memcmp(d, "abcdef", 7) == 0, "the chained result is not \"abcdef\"");
	pc_pass();
}
