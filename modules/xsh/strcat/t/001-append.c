/*
 * strcat: append.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcat.001
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strcat)(char *restrict, const char *restrict) = strcat;

int main(void)
{
	char d[16] = "ab";
	pc_begin("XSH.strcat.001");
	pc_test();
	pc_check(f_strcat(d, "cd") == d, "the return value was not the destination");
	pc_check(memcmp(d, "abcd", 5) == 0, "the result is not \"abcd\" with a terminator");
	pc_pass();
}
