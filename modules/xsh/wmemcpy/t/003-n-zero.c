/*
 * wmemcpy() with n 0 copies nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemcpy.003
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[4], src[] = { L'a', L'b' }, *r;
	int i;

	pc_begin("XSH.wmemcpy.003");
	for (i = 0; i < 4; i++)
		d[i] = L'X';
	pc_test();
	r = wmemcpy(d, src, 0);
	pc_check(r == d, "wmemcpy() did not return its first argument");
	for (i = 0; i < 4; i++)
		pc_check(d[i] == L'X', "d[%d] was changed", i);
	pc_pass();
}
