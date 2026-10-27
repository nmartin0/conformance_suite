/*
 * strncat: overwrites terminator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncat.006
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strncat)(char *restrict, const char *restrict, size_t) = strncat;

int main(void)
{
	char d[4];
	pc_begin("XSH.strncat.006");
	pc_test();
	d[0] = 'a'; d[1] = 0; d[2] = 'Z'; d[3] = 'Z';
	(void)f_strncat(d, "bc", 2);
	pc_check(d[0] == 'a' && d[1] == 'b' && d[2] == 'c' && d[3] == 0, "the result is wrong");
	pc_pass();
}
