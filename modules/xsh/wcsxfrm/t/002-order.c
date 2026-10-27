/*
 * wcscmp() on wcsxfrm() results orders strings as wcscoll() does.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsxfrm.002
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

#define DN 128
#define SIGN(v) (((v) > 0) - ((v) < 0))

static const wchar_t *const str[12] = {
	L"", L"a", L"b", L"ab", L"abc", L"A", L"B", L"0", L"9", L" ", L"z", L"Za",
};

int main(void)
{
	static wchar_t x[12][DN];
	size_t r;
	int i, j, c, k;

	pc_begin("XSH.wcsxfrm.002");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	for (i = 0; i < 12; i++) {
		r = wcsxfrm(x[i], str[i], DN);
		if (r >= DN)
			pc_unresolved("string %d needs %zu codes, more than the %d provided", i, r, DN);
	}
	for (i = 0; i < 12; i++)
		for (j = 0; j < 12; j++) {
			k = wcscmp(x[i], x[j]);
			c = wcscoll(str[i], str[j]);
			pc_check(SIGN(k) == SIGN(c), "strings %d and %d: wcscmp() of the results gave %d, wcscoll() gave %d", i, j, k, c);
		}
	pc_pass();
}
