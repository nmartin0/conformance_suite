/*
 * wmemcmp() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemcmp.005
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

/* The result is kept so that the call cannot be dropped as unused. */
static volatile long sink;

static wchar_t a[] = { L'a', L'b' }, b[] = { L'a', L'c' };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wmemcmp.005");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		sink = (long)wmemcmp(a, a, 2);
		e = errno;
		pc_check(e == before[i], "wmemcmp() (equal) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		sink = (long)wmemcmp(a, b, 2);
		e = errno;
		pc_check(e == before[i], "wmemcmp() (different) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
