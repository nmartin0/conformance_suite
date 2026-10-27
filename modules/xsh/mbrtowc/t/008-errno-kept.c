/*
 * A successful mbrtowc() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtowc.008
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	wchar_t wc;
	size_t r1, r0;
	int e1, e0;

	pc_begin("XSH.mbrtowc.008");
	memset(&st, 0, sizeof st);
	pc_test();
	errno = EDOM;
	r1 = mbrtowc(&wc, "A", 1, &st);
	e1 = errno;
	errno = EDOM;
	r0 = mbrtowc(&wc, "", 1, &st);
	e0 = errno;
	pc_check(r1 == 1 && r0 == 0, "mbrtowc() returned %zu and %zu, expected 1 and 0", r1, r0);
	pc_check(e1 == EDOM, "mbrtowc() of 'A' changed errno from EDOM to %s", pc_errname(e1));
	pc_check(e0 == EDOM, "mbrtowc() of the null byte changed errno from EDOM to %s", pc_errname(e0));
	pc_pass();
}
