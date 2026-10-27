/*
 * wctob() returns the byte that btowc() was given, for every byte value, in
 * the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctob.001
 */
#include "pc.h"
#include <stdio.h>
#include <wchar.h>

int main(void)
{
	wint_t w[256];
	int b, r, badb = -1, badr = 0;

	pc_begin("XSH.wctob.001");
	for (b = 0; b <= 255; b++) {
		w[b] = btowc(b);
		pc_check(w[b] != WEOF, "btowc(%d) returned WEOF", b);
	}
	pc_test();
	for (b = 0; b <= 255 && badb < 0; b++) {
		r = wctob(w[b]);
		if (r != b) {
			badb = b;
			badr = r;
		}
	}
	pc_check(badb < 0, "wctob() of the wide character of byte %d returned %d", badb, badr);
	pc_pass();
}
