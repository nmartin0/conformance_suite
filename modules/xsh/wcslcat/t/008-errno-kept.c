/*
 * wcslcpy() and wcslcat() leave errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslcat.008
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t src[] = { L'c', L'd', L'e', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wcslcat.008");
	pc_test();
	for (i = 0; i < 2; i++) {
		wchar_t d[8] = { L'a', L'b', 0 };

		errno = before[i];
		(void)wcslcpy(d, src, 8);
		e = errno;
		pc_check(e == before[i], "wcslcpy() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcslcpy(d, src, 3);
		e = errno;
		pc_check(e == before[i], "wcslcpy() (truncating) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcslcat(d, src, 8);
		e = errno;
		pc_check(e == before[i], "wcslcat() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
