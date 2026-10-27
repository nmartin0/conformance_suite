/*
 * wcscpy() and wcpcpy() leave errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscpy.003
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t src[] = { L'a', L'b', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	wchar_t d[8];
	int i, e;

	pc_begin("XSH.wcscpy.003");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcscpy(d, src);
		e = errno;
		pc_check(e == before[i], "wcscpy() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcpcpy(d, src);
		e = errno;
		pc_check(e == before[i], "wcpcpy() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
