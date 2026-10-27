/*
 * wcrtomb() turns the code mbrtowc() gave for a byte back into that byte,
 * for every byte, in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcrtomb.002
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	wchar_t wc[256];
	char buf[8], c;
	size_t r, badr = 0;
	int b, badb = -1;

	pc_begin("XSH.wcrtomb.002");
	memset(&st, 0, sizeof st);
	for (b = 0; b <= 255; b++) {
		c = (char)b;
		r = mbrtowc(&wc[b], &c, 1, &st);
		pc_check(r == (b == 0 ? 0u : 1u), "setup: mbrtowc() of byte %d returned %zu", b, r);
	}
	memset(&st, 0, sizeof st);
	pc_test();
	for (b = 0; b <= 255 && badb < 0; b++) {
		buf[0] = buf[1] = 'x';
		r = wcrtomb(buf, wc[b], &st);
		if (r != 1 || (unsigned char)buf[0] != b) {
			badb = b;
			badr = r;
		}
	}
	pc_check(badb < 0, "wcrtomb() of the code of byte %d returned %zu or stored another byte", badb, badr);
	pc_pass();
}
