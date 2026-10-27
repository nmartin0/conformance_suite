/*
 * wctob() maps a portable-character-set member back to its character value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctob.002
 */
#include "pc.h"
#include <stdio.h>
#include <wchar.h>

/* every member of the portable character set that has a graphic or control form */
static const char portable[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
	"abcdefghijklmnopqrstuvwxyz{|}~\a\b\t\n\v\f\r";

int main(void)
{
	const char *p;
	int r;

	pc_begin("XSH.wctob.002");
	pc_test();
	for (p = portable; *p != '\0'; p++) {
		r = wctob((wint_t)(unsigned char)*p);
		pc_check(r == (unsigned char)*p, "wctob(%d) returned %d", *p, r);
	}
	r = wctob(0);
	pc_check(r == 0, "wctob(0) returned %d, expected 0", r);
	pc_pass();
}
