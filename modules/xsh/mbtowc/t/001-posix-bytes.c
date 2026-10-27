/*
 * In the POSIX locale mbtowc() converts every byte as a one-byte character,
 * and a portable-character-set member gets its own value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbtowc.001
 */
#include "pc.h"
#include <stdlib.h>

/* every member of the portable character set that has a graphic or control form */
static const char portable[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
	"abcdefghijklmnopqrstuvwxyz{|}~\a\b\t\n\v\f\r";

int main(void)
{
	const char *p;
	wchar_t wc;
	char c;
	int b, r, badb = -1, badr = 0;

	pc_begin("XSH.mbtowc.001");
	pc_test();
	for (b = 1; b <= 255 && badb < 0; b++) {
		c = (char)b;
		r = mbtowc(&wc, &c, 1);
		if (r != 1) {
			badb = b;
			badr = r;
		}
	}
	pc_check(badb < 0, "mbtowc() of byte %d returned %d, expected 1", badb, badr);
	for (p = portable; *p != '\0'; p++) {
		wc = 77;
		r = mbtowc(&wc, p, 1);
		pc_check(r == 1 && wc == (wchar_t)(unsigned char)*p, "mbtowc() of character %d returned %d and stored %ld", *p, r, (long)wc);
	}
	pc_pass();
}
