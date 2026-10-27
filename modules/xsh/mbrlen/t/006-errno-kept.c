/*
 * A successful mbrlen() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrlen.006
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	size_t r;
	int e1, e2;

	pc_begin("XSH.mbrlen.006");
	memset(&st, 0, sizeof st);
	pc_test();
	errno = EDOM;
	r = mbrlen("A", 1, &st);
	e1 = errno;
	pc_check(r == 1, "mbrlen(\"A\") returned %zu, expected 1", r);
	pc_check(e1 == EDOM, "mbrlen() changed errno from EDOM to %s", pc_errname(e1));
	errno = EDOM;
	r = mbrlen("", 1, &st);
	e2 = errno;
	pc_check(r == 0, "mbrlen() of the null byte returned %zu, expected 0", r);
	pc_check(e2 == EDOM, "mbrlen() changed errno from EDOM to %s", pc_errname(e2));
	pc_pass();
}
