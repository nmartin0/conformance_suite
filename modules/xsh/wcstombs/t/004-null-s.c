/*
 * With a null s wcstombs() returns the length required, whatever n is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstombs.004
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	size_t r1, r2;

	pc_begin("XSH.wcstombs.004");
	pc_test();
	r1 = wcstombs(NULL, L"abcd", 1);
	r2 = wcstombs(NULL, L"abcd", 100);
	pc_check(r1 == 4, "wcstombs(NULL, L\"abcd\", 1) returned %zu, expected 4", r1);
	pc_check(r2 == 4, "wcstombs(NULL, L\"abcd\", 100) returned %zu, expected 4", r2);
	pc_pass();
}
