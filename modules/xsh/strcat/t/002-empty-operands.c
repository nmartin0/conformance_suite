/*
 * strcat: empty operands.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcat.002
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strcat)(char *restrict, const char *restrict) = strcat;

int main(void)
{
	char d[16];
	pc_begin("XSH.strcat.002");
	pc_test();
	memset(d, 'X', sizeof d); strcpy(d, "ab");
	pc_check(f_strcat(d, "") == d, "empty source: wrong return value");
	pc_check(memcmp(d, "ab", 3) == 0, "empty source changed the string");
	memset(d, 'X', sizeof d); d[0] = 0;
	pc_check(f_strcat(d, "xyz") == d, "empty destination: wrong return value");
	pc_check(memcmp(d, "xyz", 4) == 0, "empty destination: wrong result");
	memset(d, 'X', sizeof d); d[0] = 0;
	pc_check(f_strcat(d, "") == d && d[0] == 0, "two empty strings: wrong result");
	pc_pass();
}
