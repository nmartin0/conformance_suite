/*
 * A successful wcrtomb() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcrtomb.006
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	char buf[8];
	size_t r1, r0;
	int e1, e0;

	pc_begin("XSH.wcrtomb.006");
	memset(&st, 0, sizeof st);
	pc_test();
	errno = EDOM;
	r1 = wcrtomb(buf, L'A', &st);
	e1 = errno;
	errno = EDOM;
	r0 = wcrtomb(buf, 0, &st);
	e0 = errno;
	pc_check(r1 == 1 && r0 == 1, "wcrtomb() returned %zu and %zu, expected 1 and 1", r1, r0);
	pc_check(e1 == EDOM, "wcrtomb() of L'A' changed errno from EDOM to %s", pc_errname(e1));
	pc_check(e0 == EDOM, "wcrtomb() of the null wide character changed errno from EDOM to %s", pc_errname(e0));
	pc_pass();
}
