/*
 * wcscmp() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscmp.004
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

/* The result is kept so that the call cannot be dropped as unused. */
static volatile long sink;

static wchar_t a[] = { L'a', L'b', 0 }, b[] = { L'a', L'c', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wcscmp.004");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		sink = (long)wcscmp(a, a);
		e = errno;
		pc_check(e == before[i], "wcscmp() (equal) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		sink = (long)wcscmp(a, b);
		e = errno;
		pc_check(e == before[i], "wcscmp() (different) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
