/*
 * wcsxfrm() and wcsxfrm_l() leave errno alone when they succeed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsxfrm.006
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <wchar.h>

int main(void)
{
	static const int before[2] = { 0, EDOM };
	wchar_t d[128];
	locale_t loc;
	int i, e;

	pc_begin("XSH.wcsxfrm.006");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (loc == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcsxfrm(d, L"abc", 128);
		e = errno;
		pc_check(e == before[i], "wcsxfrm() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcsxfrm_l(d, L"abc", 128, loc);
		e = errno;
		pc_check(e == before[i], "wcsxfrm_l() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	freelocale(loc);
	pc_pass();
}
