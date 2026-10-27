/*
 * wmemset() stores the null code and WCHAR_MAX like any other value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemset.003
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t a[8], b[8];
	int i;

	pc_begin("XSH.wmemset.003");
	for (i = 0; i < 8; i++)
		a[i] = b[i] = L'X';
	pc_test();
	(void)wmemset(a, 0, 3);
	(void)wmemset(b, WCHAR_MAX, 3);
	for (i = 0; i < 3; i++) {
		pc_check(a[i] == 0, "a[%d] was not set to the null code", i);
		pc_check(b[i] == WCHAR_MAX, "b[%d] was not set to WCHAR_MAX", i);
	}
	for (i = 3; i < 8; i++) {
		pc_check(a[i] == L'X', "a[%d] after the n elements was changed", i);
		pc_check(b[i] == L'X', "b[%d] after the n elements was changed", i);
	}
	pc_pass();
}
