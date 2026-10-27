/*
 * wcsncmp() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncmp.005
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t a[] = { L'a', L'b', 0 }, b[] = { L'a', L'c', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wcsncmp.005");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcsncmp(a, a, 5);
		e = errno;
		pc_check(e == before[i], "wcsncmp() (equal) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcsncmp(a, b, 5);
		e = errno;
		pc_check(e == before[i], "wcsncmp() (different) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
