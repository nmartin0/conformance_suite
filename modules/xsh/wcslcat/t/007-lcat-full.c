/*
 * wcslcat() onto a destination string that just fills the size.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslcat.007
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[12], src[] = { L'x', L'y', L'z', 0 };
	size_t r;
	int i;

	pc_begin("XSH.wcslcat.007");
	for (i = 0; i < 12; i++)
		d[i] = L'X';
	d[0] = L'a';
	d[1] = L'b';
	d[2] = L'c';
	d[3] = 0;
	pc_test();
	r = wcslcat(d, src, 4);
	pc_check(r == 6, "wcslcat() returned %zu, expected 6", r);
	pc_check(d[0] == L'a' && d[1] == L'b' && d[2] == L'c' && d[3] == 0, "the destination \"abc\" was changed");
	for (i = 4; i < 12; i++)
		pc_check(d[i] == L'X', "d[%d] after the destination was changed", i);
	pc_pass();
}
