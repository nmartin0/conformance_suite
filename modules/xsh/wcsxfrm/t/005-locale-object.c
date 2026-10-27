/*
 * wcsxfrm_l() with a locale object for the POSIX locale agrees with wcscoll_l().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsxfrm.005
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

#define DN 128
#define SIGN(v) (((v) > 0) - ((v) < 0))

static const wchar_t *const str[6] = { L"", L"a", L"b", L"ab", L"A", L"9" };

int main(void)
{
	static wchar_t x[6][DN];
	locale_t loc;
	size_t r;
	int i, j, c, k;

	pc_begin("XSH.wcsxfrm.005");
	loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (loc == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	for (i = 0; i < 6; i++) {
		r = wcsxfrm_l(x[i], str[i], DN, loc);
		if (r >= DN)
			pc_unresolved("string %d needs %zu codes, more than the %d provided", i, r, DN);
	}
	for (i = 0; i < 6; i++)
		for (j = 0; j < 6; j++) {
			k = wcscmp(x[i], x[j]);
			c = wcscoll_l(str[i], str[j], loc);
			pc_check(SIGN(k) == SIGN(c), "strings %d and %d: wcscmp() of the results gave %d, wcscoll_l() gave %d", i, j, k, c);
		}
	freelocale(loc);
	pc_pass();
}
