/*
 * mbtowc(NULL, NULL, 0) returns 0 in the POSIX locale (no state-dependent
 * encoding).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbtowc.003
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	int r;

	pc_begin("XSH.mbtowc.003");
	pc_test();
	r = mbtowc(NULL, NULL, 0);
	pc_check(r == 0, "mbtowc(NULL, NULL, 0) returned %d, expected 0", r);
	pc_pass();
}
