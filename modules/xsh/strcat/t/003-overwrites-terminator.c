/*
 * strcat: overwrites terminator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcat.003
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strcat)(char *restrict, const char *restrict) = strcat;

int main(void)
{
	char d[6];
	pc_begin("XSH.strcat.003");
	pc_test();
	d[0] = 'a'; d[1] = 0; d[2] = 'Z'; d[3] = 'Z'; d[4] = 'Z'; d[5] = 'Z';
	(void)f_strcat(d, "bc");
	pc_check(d[0] == 'a' && d[1] == 'b' && d[2] == 'c' && d[3] == 0, "the old terminator was not replaced by the first source byte");
	pc_pass();
}
