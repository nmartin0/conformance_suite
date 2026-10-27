/*
 * mbtowc() with a null pwc still returns the length; the null byte stores
 * the wide-character code 0 and returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbtowc.002
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	wchar_t wc = 77;
	int r1, r2;

	pc_begin("XSH.mbtowc.002");
	pc_test();
	r1 = mbtowc(NULL, "A", 1);
	r2 = mbtowc(&wc, "", 1);
	pc_check(r1 == 1, "mbtowc(NULL, \"A\", 1) returned %d, expected 1", r1);
	pc_check(r2 == 0, "mbtowc() of the null byte returned %d, expected 0", r2);
	pc_check(wc == 0, "mbtowc() of the null byte stored %ld, expected 0", (long)wc);
	pc_pass();
}
