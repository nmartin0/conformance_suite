/*
 * wcslcpy() truncates to size-1 codes, ends with a null, returns the source length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslcat.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t abcdef[] = { L'a', L'b', L'c', L'd', L'e', L'f', 0 };
static wchar_t abcd[] = { L'a', L'b', L'c', L'd', 0 };
static wchar_t abc[] = { L'a', L'b', L'c', 0 };

int main(void)
{
	static const size_t size[3] = { 4, 4, 4 }, want[3] = { 6, 4, 3 }, stored[3] = { 3, 3, 3 };
	const wchar_t *src[3];
	wchar_t d[16];
	size_t r;
	int i, k;

	pc_begin("XSH.wcslcat.002");
	src[0] = abcdef;
	src[1] = abcd;
	src[2] = abc;
	pc_test();
	for (k = 0; k < 3; k++) {
		for (i = 0; i < 16; i++)
			d[i] = L'X';
		r = wcslcpy(d, src[k], size[k]);
		pc_check(r == want[k], "case %d: wcslcpy() returned %zu, expected %zu", k, r, want[k]);
		for (i = 0; i < (int)stored[k]; i++)
			pc_check(d[i] == src[k][i], "case %d: d[%d] is not the source code", k, i);
		pc_check(d[stored[k]] == 0, "case %d: no null code ends the result", k);
		for (i = (int)stored[k] + 1; i < 16; i++)
			pc_check(d[i] == L'X', "case %d: d[%d] after the result was changed", k, i);
	}
	pc_pass();
}
