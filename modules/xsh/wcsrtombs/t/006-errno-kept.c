/*
 * A successful wcsrtombs() or wcsnrtombs() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrtombs.006
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	const wchar_t *src = L"abc";
	char d[8];
	size_t r1, r2;
	int e1, e2;

	pc_begin("XSH.wcsrtombs.006");
	memset(&st, 0, sizeof st);
	pc_test();
	errno = EDOM;
	r1 = wcsrtombs(d, &src, 8, &st);
	e1 = errno;
	src = L"abc";
	errno = EDOM;
	r2 = wcsnrtombs(d, &src, 3, 8, &st);
	e2 = errno;
	pc_check(r1 == 3 && r2 == 3, "wcsrtombs() and wcsnrtombs() returned %zu and %zu, expected 3 and 3", r1, r2);
	pc_check(e1 == EDOM, "wcsrtombs() changed errno from EDOM to %s", pc_errname(e1));
	pc_check(e2 == EDOM, "wcsnrtombs() changed errno from EDOM to %s", pc_errname(e2));
	pc_pass();
}
