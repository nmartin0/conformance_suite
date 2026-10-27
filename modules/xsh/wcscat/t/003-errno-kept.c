/*
 * wcscat() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscat.003
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

int main(void)
{
	static const int before[2] = { 0, EDOM };
	wchar_t src[] = { L'c', L'd', 0 };
	int i, e;

	pc_begin("XSH.wcscat.003");
	pc_test();
	for (i = 0; i < 2; i++) {
		wchar_t d[8] = { L'a', L'b', 0 };

		errno = before[i];
		(void)wcscat(d, src);
		e = errno;
		pc_check(e == before[i], "wcscat() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
