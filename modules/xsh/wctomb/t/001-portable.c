/*
 * wctomb() stores the one byte of a portable-character-set member, and the
 * null wide-character code gives a null byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctomb.001
 */
#include "pc.h"
#include <stdlib.h>

/* every member of the portable character set that has a graphic or control form */
static const char portable[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
	"abcdefghijklmnopqrstuvwxyz{|}~\a\b\t\n\v\f\r";

int main(void)
{
	const char *p;
	char buf[8];
	int r;

	pc_begin("XSH.wctomb.001");
	pc_test();
	for (p = portable; *p != '\0'; p++) {
		buf[0] = buf[1] = 'x';
		r = wctomb(buf, (wchar_t)(unsigned char)*p);
		pc_check(r == 1 && buf[0] == *p, "wctomb() of character %d returned %d and stored %d", *p, r, buf[0]);
	}
	buf[0] = 'x';
	r = wctomb(buf, 0);
	pc_check(r == 1, "wctomb() of the null wide character returned %d, expected 1", r);
	pc_check(buf[0] == '\0', "wctomb() of the null wide character stored %d, expected a null byte", buf[0]);
	pc_pass();
}
