/*
 * In the POSIX locale mbrtowc() converts every non-null byte as a one-byte
 * character; a portable-character-set member gets its own value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtowc.001
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
	wchar_t wc;
	char c;
	size_t r, badr = 0;
	int b, badb = -1;

	pc_begin("XSH.mbrtowc.001");
	memset(&st, 0, sizeof st);
	pc_test();
	for (b = 1; b <= 255 && badb < 0; b++) {
		c = (char)b;
		r = mbrtowc(&wc, &c, 1, &st);
		if (r != 1) {
			badb = b;
			badr = r;
		}
	}
	pc_check(badb < 0, "mbrtowc() of byte %d returned %zu, expected 1", badb, badr);
	for (p = portable; *p != '\0'; p++) {
		wc = 77;
		r = mbrtowc(&wc, p, 1, &st);
		pc_check(r == 1 && wc == (wchar_t)(unsigned char)*p, "mbrtowc() of character %d returned %zu and stored %ld", *p, r, (long)wc);
	}
	pc_pass();
}
