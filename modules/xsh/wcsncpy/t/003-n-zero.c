/*
 * wcsncpy() with n 0 writes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncpy.003
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[4], src[] = { L'a', L'b', 0 }, *r;
	int i;

	pc_begin("XSH.wcsncpy.003");
	for (i = 0; i < 4; i++)
		d[i] = L'X';
	pc_test();
	r = wcsncpy(d, src, 0);
	pc_check(r == d, "wcsncpy() did not return its first argument");
	for (i = 0; i < 4; i++)
		pc_check(d[i] == L'X', "d[%d] was changed", i);
	pc_pass();
}
