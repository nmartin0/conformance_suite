/*
 * wcscasecmp_l() and wcsncasecmp_l() with a locale object for the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscasecmp.004
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

#define SGN(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(v) (((v) > 0) - ((v) < 0))

int main(void)
{
	locale_t loc;
	int r[4];

	pc_begin("XSH.wcscasecmp.004");
	loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (loc == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	r[0] = wcscasecmp_l(L"abc", L"ABC", loc);
	r[1] = wcscasecmp_l(L"a", L"B", loc);
	r[2] = wcsncasecmp_l(L"abcd", L"ABXY", 2, loc);
	r[3] = wcsncasecmp_l(L"abcd", L"ABXY", 3, loc);
	pc_check(r[0] == 0, "wcscasecmp_l(\"abc\", \"ABC\") returned %d", r[0]);
	pc_check(SIGN(r[1]) == SGN(L'a', L'b'), "wcscasecmp_l(\"a\", \"B\") returned %d", r[1]);
	pc_check(r[2] == 0, "wcsncasecmp_l() with n 2 returned %d", r[2]);
	pc_check(SIGN(r[3]) == SGN(L'c', L'x'), "wcsncasecmp_l() with n 3 returned %d", r[3]);
	freelocale(loc);
	pc_pass();
}
