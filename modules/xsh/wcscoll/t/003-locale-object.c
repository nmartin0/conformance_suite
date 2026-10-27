/*
 * wcscoll_l() with a locale object for the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscoll.003
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

static const struct { const wchar_t *first, *second; } pair[] = {
	{ L"a", L"b" }, { L"A", L"a" }, { L"ab", L"abc" },
};
#define NPAIR (sizeof pair / sizeof pair[0])
static wchar_t e1[] = { L'a', L'b', 0 }, e2[] = { L'a', L'b', 0 };

int main(void)
{
	locale_t loc;
	size_t i;
	int f, b, eq;

	pc_begin("XSH.wcscoll.003");
	loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (loc == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	eq = wcscoll_l(e1, e2, loc);
	pc_check(eq == 0, "wcscoll_l() of equal strings returned %d", eq);
	for (i = 0; i < NPAIR; i++) {
		f = wcscoll_l(pair[i].first, pair[i].second, loc);
		b = wcscoll_l(pair[i].second, pair[i].first, loc);
		pc_check(f < 0, "pair %zu: wcscoll_l(first, second) returned %d, expected negative", i, f);
		pc_check(b > 0, "pair %zu: wcscoll_l(second, first) returned %d, expected positive", i, b);
	}
	freelocale(loc);
	pc_pass();
}
