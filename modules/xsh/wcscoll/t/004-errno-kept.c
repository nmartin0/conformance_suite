/*
 * wcscoll() and wcscoll_l() leave errno alone when they succeed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscoll.004
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static wchar_t a[] = { L'a', L'b', 0 }, b[] = { L'a', L'c', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	locale_t loc;
	int i, e;

	pc_begin("XSH.wcscoll.004");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (loc == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcscoll(a, b);
		e = errno;
		pc_check(e == before[i], "wcscoll() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcscoll_l(a, b, loc);
		e = errno;
		pc_check(e == before[i], "wcscoll_l() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	freelocale(loc);
	pc_pass();
}
