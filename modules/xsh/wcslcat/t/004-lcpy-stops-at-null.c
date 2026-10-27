/*
 * wcslcpy() stops at the source null code and does not pad.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslcat.004
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[12], src[] = { L'a', L'b', 0, L'c' };
	size_t r;
	int i;

	pc_begin("XSH.wcslcat.004");
	for (i = 0; i < 12; i++)
		d[i] = L'X';
	pc_test();
	r = wcslcpy(d, src, 8);
	pc_check(r == 2, "wcslcpy() returned %zu, expected 2", r);
	pc_check(d[0] == L'a' && d[1] == L'b' && d[2] == 0, "the result is not \"ab\" and a null code");
	for (i = 3; i < 12; i++)
		pc_check(d[i] == L'X', "d[%d] after the result was changed", i);
	pc_pass();
}
