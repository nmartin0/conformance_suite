/*
 * wcsxfrm() returns the length of the transformed string it stores.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsxfrm.001
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

#define DN 128

int main(void)
{
	wchar_t d[DN];
	size_t r, len;
	int i;

	pc_begin("XSH.wcsxfrm.001");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	for (i = 0; i < DN; i++)
		d[i] = L'X';
	pc_test();
	r = wcsxfrm(d, L"abc", DN);
	if (r >= DN)
		pc_unresolved("the transformed string needs %zu codes, more than the %d provided", r, DN);
	len = wcslen(d);
	pc_check(len == r, "wcsxfrm() returned %zu but the result has length %zu (or no null code)", r, len);
	pc_check(d[r] == 0, "no null code at index %zu", r);
	pc_pass();
}
