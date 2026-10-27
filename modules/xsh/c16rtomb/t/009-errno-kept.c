/*
 * A successful c32rtomb() or c16rtomb() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.c16rtomb.009
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <uchar.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	char buf[8];
	size_t r32, r16;
	int e32, e16;

	pc_begin("XSH.c16rtomb.009");
	memset(&st, 0, sizeof st);
	pc_test();
	errno = EDOM;
	r32 = c32rtomb(buf, 0, &st);
	e32 = errno;
	errno = EDOM;
	r16 = c16rtomb(buf, 0, &st);
	e16 = errno;
	pc_check(r32 == 1 && r16 == 1, "c32rtomb() and c16rtomb() returned %zu and %zu, expected 1 and 1", r32, r16);
	pc_check(e32 == EDOM, "c32rtomb() changed errno from EDOM to %s", pc_errname(e32));
	pc_check(e16 == EDOM, "c16rtomb() changed errno from EDOM to %s", pc_errname(e16));
	pc_pass();
}
