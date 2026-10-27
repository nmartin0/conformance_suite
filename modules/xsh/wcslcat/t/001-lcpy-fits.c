/*
 * wcslcpy() copies a string that fits and returns its length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslcat.001
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[16], src[] = { L'a', L'b', L'c', 0 };
	size_t r;
	int i;

	pc_begin("XSH.wcslcat.001");
	for (i = 0; i < 16; i++)
		d[i] = L'X';
	pc_test();
	r = wcslcpy(d, src, 10);
	pc_check(r == 3, "wcslcpy() returned %zu, expected 3", r);
	pc_check(d[0] == L'a' && d[1] == L'b' && d[2] == L'c', "the string was not copied");
	pc_check(d[3] == 0, "the null code was not stored");
	for (i = 4; i < 16; i++)
		pc_check(d[i] == L'X', "d[%d] after the result was changed", i);
	pc_pass();
}
