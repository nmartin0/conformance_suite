/*
 * wmemcpy() copies every wchar_t value unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemcpy.004
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[6], src[] = { 1, WCHAR_MAX, 0 };
	int i;

	pc_begin("XSH.wmemcpy.004");
	for (i = 0; i < 6; i++)
		d[i] = L'X';
	pc_test();
	(void)wmemcpy(d, src, 3);
	pc_check(d[0] == 1, "the element 1 was changed");
	pc_check(d[1] == WCHAR_MAX, "the element WCHAR_MAX was changed");
	pc_check(d[2] == 0, "the null element was not copied");
	for (i = 3; i < 6; i++)
		pc_check(d[i] == L'X', "d[%d] after the n elements was changed", i);
	pc_pass();
}
