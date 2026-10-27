/*
 * wcschr() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcschr.004
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t ws[] = { L'a', L'b', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wcschr.004");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcschr(ws, L'b');
		e = errno;
		pc_check(e == before[i], "wcschr() (found) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcschr(ws, L'z');
		e = errno;
		pc_check(e == before[i], "wcschr() (not found) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
