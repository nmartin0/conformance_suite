/*
 * wcsncpy() pads a short source with null codes up to n codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncpy.001
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[12], src[] = { L'a', L'b', 0, L'c', L'd', 0 }, *r;
	int i;

	pc_begin("XSH.wcsncpy.001");
	for (i = 0; i < 12; i++)
		d[i] = L'X';
	pc_test();
	r = wcsncpy(d, src, 6);
	pc_check(r == d, "wcsncpy() did not return its first argument");
	pc_check(d[0] == L'a' && d[1] == L'b', "the string was not copied");
	for (i = 2; i < 6; i++)
		pc_check(d[i] == 0, "d[%d] is not a null code (padding or a code after the source null)", i);
	for (i = 6; i < 12; i++)
		pc_check(d[i] == L'X', "d[%d] after the n codes was changed", i);
	pc_pass();
}
