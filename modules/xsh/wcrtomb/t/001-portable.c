/*
 * wcrtomb() stores the one byte of a portable-character-set member, with an
 * explicit or a null ps; the null wide character gives a null byte and the
 * initial state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcrtomb.001
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

/* every member of the portable character set that has a graphic or control form */
static const char portable[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
	"abcdefghijklmnopqrstuvwxyz{|}~\a\b\t\n\v\f\r";

int main(void)
{
	mbstate_t st;
	const char *p;
	char buf[8];
	size_t r, r0, rn;
	int init;

	pc_begin("XSH.wcrtomb.001");
	memset(&st, 0, sizeof st);
	pc_test();
	for (p = portable; *p != '\0'; p++) {
		buf[0] = buf[1] = 'x';
		r = wcrtomb(buf, (wchar_t)(unsigned char)*p, &st);
		pc_check(r == 1 && buf[0] == *p, "wcrtomb() of character %d returned %zu and stored %d", *p, r, buf[0]);
		buf[0] = buf[1] = 'x';
		r = wcrtomb(buf, (wchar_t)(unsigned char)*p, NULL);
		pc_check(r == 1 && buf[0] == *p, "wcrtomb() with a null ps of character %d returned %zu and stored %d", *p, r, buf[0]);
	}
	buf[0] = 'x';
	r0 = wcrtomb(buf, 0, &st);
	init = mbsinit(&st);
	pc_check(r0 == 1 && buf[0] == '\0', "wcrtomb() of the null wide character returned %zu and stored %d", r0, buf[0]);
	pc_check(init != 0, "wcrtomb() of the null wide character left a state that is not initial");
	buf[0] = 'x';
	rn = wcrtomb(buf, 0, NULL);
	pc_check(rn == 1 && buf[0] == '\0', "wcrtomb() with a null ps of the null wide character returned %zu and stored %d", rn, buf[0]);
	pc_pass();
}
