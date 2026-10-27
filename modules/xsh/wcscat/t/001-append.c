/*
 * wcscat() appends the source, null code included, and returns the destination.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscat.001
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[16], src[] = { L'c', L'd', 0 }, *r;
	int i;

	pc_begin("XSH.wcscat.001");
	for (i = 0; i < 16; i++)
		d[i] = L'X';
	d[0] = L'a';
	d[1] = L'b';
	d[2] = 0;
	pc_test();
	r = wcscat(d, src);
	pc_check(r == d, "wcscat() did not return its first argument");
	pc_check(d[0] == L'a' && d[1] == L'b', "the original string was changed");
	pc_check(d[2] == L'c' && d[3] == L'd', "the source was not appended at the old null code");
	pc_check(d[4] == 0, "the null code of the source was not copied to index 4");
	for (i = 5; i < 16; i++)
		pc_check(d[i] == L'X', "d[%d] after the result was changed", i);
	pc_pass();
}
