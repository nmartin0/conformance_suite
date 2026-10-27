/*
 * A successful mbsrtowcs() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsrtowcs.005
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	const char *src = "abc";
	wchar_t w[8];
	size_t r;
	int e;

	pc_begin("XSH.mbsrtowcs.005");
	memset(&st, 0, sizeof st);
	pc_test();
	errno = EDOM;
	r = mbsrtowcs(w, &src, 8, &st);
	e = errno;
	pc_check(r == 3, "mbsrtowcs() returned %zu, expected 3", r);
	pc_check(e == EDOM, "mbsrtowcs() changed errno from EDOM to %s", pc_errname(e));
	pc_pass();
}
