/*
 * wcsncpy() and wcpncpy() leave errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncpy.005
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

	pc_begin("XSH.wcsncpy.005");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcsncpy(d, src, 5);
		e = errno;
		pc_check(e == before[i], "wcsncpy() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcpncpy(d, src, 5);
		e = errno;
		pc_check(e == before[i], "wcpncpy() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
