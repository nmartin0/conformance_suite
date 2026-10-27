/*
 * wcslen() and wcsnlen() leave errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslen.004
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', L'c', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wcslen.004");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcslen(s);
		e = errno;
		pc_check(e == before[i], "wcslen() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcsnlen(s, 2);
		e = errno;
		pc_check(e == before[i], "wcsnlen() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcsnlen(s, 10);
		e = errno;
		pc_check(e == before[i], "wcsnlen() (long maxlen) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
