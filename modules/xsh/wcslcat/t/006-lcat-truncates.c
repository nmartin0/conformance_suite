/*
 * wcslcat() truncates to fit the size and returns the length it tried to make.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslcat.006
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[16], src[] = { L'c', L'd', L'e', L'f', L'g', L'h', 0 };
	size_t r;
	int i;

	pc_begin("XSH.wcslcat.006");
	for (i = 0; i < 16; i++)
		d[i] = L'X';
	d[0] = L'a';
	d[1] = L'b';
	d[2] = 0;
	pc_test();
	r = wcslcat(d, src, 6);
	pc_check(r == 8, "wcslcat() returned %zu, expected 8", r);
	pc_check(d[0] == L'a' && d[1] == L'b' && d[2] == L'c' && d[3] == L'd' && d[4] == L'e',
	    "the result is not \"abcde\"");
	pc_check(d[5] == 0, "no null code ends the result at index 5");
	for (i = 6; i < 16; i++)
		pc_check(d[i] == L'X', "d[%d] after the result was changed", i);
	pc_pass();
}
