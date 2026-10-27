/*
 * wmemset() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemset.004
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

int main(void)
{
	static const int before[2] = { 0, EDOM };
	wchar_t buf[6];
	int i, e;

	pc_begin("XSH.wmemset.004");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wmemset(buf, L'q', 6);
		e = errno;
		pc_check(e == before[i], "wmemset() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
