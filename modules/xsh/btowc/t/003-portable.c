/*
 * btowc() gives a portable-character-set member its own character value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.btowc.003
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
	wint_t r;

	pc_begin("XSH.btowc.003");
	pc_test();
	for (p = portable; *p != '\0'; p++) {
		r = btowc((unsigned char)*p);
		pc_check(r == (wint_t)(unsigned char)*p, "btowc(%d) returned %lu", *p, (unsigned long)r);
	}
	r = btowc(0);
	pc_check(r == 0, "btowc(0) returned %lu, expected 0", (unsigned long)r);
	pc_pass();
}
