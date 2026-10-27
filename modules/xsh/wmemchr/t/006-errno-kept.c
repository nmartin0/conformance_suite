/*
 * wmemchr() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemchr.006
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t w[] = { L'a', L'b', L'c' };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wmemchr.006");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wmemchr(w, L'b', 3);
		e = errno;
		pc_check(e == before[i], "wmemchr() (found) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wmemchr(w, L'z', 3);
		e = errno;
		pc_check(e == before[i], "wmemchr() (not found) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
