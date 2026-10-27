/*
 * mblen(NULL, 0) returns 0 in the POSIX locale (no state-dependent encoding).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mblen.001
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	int r;

	pc_begin("XSH.mblen.001");
	pc_test();
	r = mblen(NULL, 0);
	pc_check(r == 0, "mblen(NULL, 0) returned %d, expected 0", r);
	pc_pass();
}
