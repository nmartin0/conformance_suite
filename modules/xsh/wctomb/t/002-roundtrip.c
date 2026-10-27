/*
 * wctomb() turns the code mbtowc() gave for a byte back into that byte, for
 * every byte, in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctomb.002
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	wchar_t wc[256];
	char buf[8], c;
	int b, r, badb = -1, badr = 0;

	pc_begin("XSH.wctomb.002");
	for (b = 0; b <= 255; b++) {
		c = (char)b;
		r = mbtowc(&wc[b], &c, 1);
		pc_check(r == (b == 0 ? 0 : 1), "setup: mbtowc() of byte %d returned %d", b, r);
	}
	pc_test();
	for (b = 0; b <= 255 && badb < 0; b++) {
		buf[0] = buf[1] = 'x';
		r = wctomb(buf, wc[b]);
		if (r != 1 || (unsigned char)buf[0] != b) {
			badb = b;
			badr = r;
		}
	}
	pc_check(badb < 0, "wctomb() of the code of byte %d returned %d or stored another byte", badb, badr);
	pc_pass();
}
