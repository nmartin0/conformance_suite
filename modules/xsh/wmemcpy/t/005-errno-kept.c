/*
 * wmemcpy() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemcpy.005
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t src[] = { L'a', L'b' };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	wchar_t d[4];
	int i, e;

	pc_begin("XSH.wmemcpy.005");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wmemcpy(d, src, 2);
		e = errno;
		pc_check(e == before[i], "wmemcpy() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wmemcpy(d, src, 0);
		e = errno;
		pc_check(e == before[i], "wmemcpy() with n 0 changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
