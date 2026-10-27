/*
 * wctomb(NULL, wc) returns 0 in the POSIX locale (no state-dependent
 * encoding).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctomb.004
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	int r;

	pc_begin("XSH.wctomb.004");
	pc_test();
	r = wctomb(NULL, L'A');
	pc_check(r == 0, "wctomb(NULL, L'A') returned %d, expected 0", r);
	pc_pass();
}
