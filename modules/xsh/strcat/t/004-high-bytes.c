/*
 * strcat: high bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcat.004
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strcat)(char *restrict, const char *restrict) = strcat;

int main(void)
{
	static const char s[4] = { (char)0x80, (char)0xff, (char)0xe9, 0 };
	unsigned char d[8];
	pc_begin("XSH.strcat.004");
	pc_test();
	memset(d, 'X', sizeof d); d[0] = 'a'; d[1] = 0;
	(void)f_strcat((char *)d, s);
	pc_check(d[0] == 'a' && d[1] == 0x80 && d[2] == 0xff && d[3] == 0xe9 && d[4] == 0, "high bytes were not preserved");
	pc_pass();
}
