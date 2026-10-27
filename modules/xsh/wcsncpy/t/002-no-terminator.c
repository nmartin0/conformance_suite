/*
 * wcsncpy() stores exactly n codes when the source has no null code in them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncpy.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t abcdef[] = { L'a', L'b', L'c', L'd', L'e', L'f', 0 };
static wchar_t abc[] = { L'a', L'b', L'c', 0 };

int main(void)
{
	const wchar_t *src[2];
	wchar_t d[8], *r;
	int i, k;

	pc_begin("XSH.wcsncpy.002");
	src[0] = abcdef;
	src[1] = abc;
	pc_test();
	for (k = 0; k < 2; k++) {
		for (i = 0; i < 8; i++)
			d[i] = L'X';
		r = wcsncpy(d, src[k], 3);
		pc_check(r == d, "case %d: wcsncpy() did not return its first argument", k);
		pc_check(d[0] == L'a' && d[1] == L'b' && d[2] == L'c', "case %d: three codes were not copied", k);
		for (i = 3; i < 8; i++)
			pc_check(d[i] == L'X', "case %d: d[%d] after the n codes was changed", k, i);
	}
	pc_pass();
}
