/*
 * wcslcat() appends a string that fits and returns the total length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslcat.005
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[16], src[] = { L'c', L'd', 0, L'e' };
	size_t r;
	int i;

	pc_begin("XSH.wcslcat.005");
	for (i = 0; i < 16; i++)
		d[i] = L'X';
	d[0] = L'a';
	d[1] = L'b';
	d[2] = 0;
	pc_test();
	r = wcslcat(d, src, 10);
	pc_check(r == 4, "wcslcat() returned %zu, expected 4", r);
	pc_check(d[0] == L'a' && d[1] == L'b' && d[2] == L'c' && d[3] == L'd', "the result is not \"abcd\"");
	pc_check(d[4] == 0, "no null code ends the result");
	for (i = 5; i < 16; i++)
		pc_check(d[i] == L'X', "d[%d] after the result was changed", i);
	pc_pass();
}
