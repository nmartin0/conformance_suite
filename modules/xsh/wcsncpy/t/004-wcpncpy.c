/*
 * wcpncpy() returns the address of the first null code written, else &ws1[n].
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncpy.004
 */
#include "pc.h"
#include <wchar.h>

static wchar_t ab[] = { L'a', L'b', 0 };
static wchar_t abcdef[] = { L'a', L'b', L'c', L'd', L'e', L'f', 0 };
static wchar_t abc[] = { L'a', L'b', L'c', 0 };

int main(void)
{
	static const size_t n[3] = { 6, 3, 3 }, at[3] = { 2, 3, 3 };
	const wchar_t *src[3];
	wchar_t d[12], *r;
	int i, k;

	pc_begin("XSH.wcsncpy.004");
	src[0] = ab;
	src[1] = abcdef;
	src[2] = abc;
	pc_test();
	for (k = 0; k < 3; k++) {
		for (i = 0; i < 12; i++)
			d[i] = L'X';
		r = wcpncpy(d, src[k], n[k]);
		pc_check(r == d + at[k], "case %d: wcpncpy() did not return &d[%zu]", k, at[k]);
		for (i = 0; i < (int)at[k]; i++)
			pc_check(d[i] == src[k][i], "case %d: d[%d] is not the source code", k, i);
		for (i = (int)at[k]; i < (int)n[k]; i++)
			pc_check(d[i] == 0 || at[k] == n[k], "case %d: d[%d] is not a padding null code", k, i);
		for (i = (int)n[k]; i < 12; i++)
			pc_check(d[i] == L'X', "case %d: d[%d] after the n codes was changed", k, i);
	}
	for (i = 0; i < 12; i++)
		d[i] = L'X';
	r = wcpncpy(d, ab, 0);
	pc_check(r == d, "wcpncpy() with n 0 did not return its first argument");
	pc_check(d[0] == L'X', "wcpncpy() with n 0 wrote a code");
	pc_pass();
}
