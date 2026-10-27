/*
 * wcsncat() onto an empty destination.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncat.004
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[8], src[] = { L'a', L'b', L'c', 0 }, *r;
	int i;

	pc_begin("XSH.wcsncat.004");
	for (i = 0; i < 8; i++)
		d[i] = L'X';
	d[0] = 0;
	pc_test();
	r = wcsncat(d, src, 2);
	pc_check(r == d, "wcsncat() did not return its first argument");
	pc_check(d[0] == L'a' && d[1] == L'b', "the result is not \"ab\"");
	pc_check(d[2] == 0, "no null code ends the result");
	for (i = 3; i < 8; i++)
		pc_check(d[i] == L'X', "d[%d] after the result was changed", i);
	pc_pass();
}
