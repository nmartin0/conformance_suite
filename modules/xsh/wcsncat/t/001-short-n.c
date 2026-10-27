/*
 * wcsncat() appends at most n codes and always ends with a null code.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncat.001
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[16], src[] = { L'c', L'd', L'e', L'f', 0 }, *r;
	int i;

	pc_begin("XSH.wcsncat.001");
	for (i = 0; i < 16; i++)
		d[i] = L'X';
	d[0] = L'a';
	d[1] = L'b';
	d[2] = 0;
	pc_test();
	r = wcsncat(d, src, 2);
	pc_check(r == d, "wcsncat() did not return its first argument");
	pc_check(d[0] == L'a' && d[1] == L'b' && d[2] == L'c' && d[3] == L'd', "the result is not \"abcd\"");
	pc_check(d[4] == 0, "no null code ends the result");
	for (i = 5; i < 16; i++)
		pc_check(d[i] == L'X', "d[%d] after the result was changed", i);
	pc_pass();
}
