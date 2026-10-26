/*
 * stpcpy() and strcpy() leave errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.stpcpy.003
 * MUTANT    stpcpy.errno_clobbered
 * MUTANT    stpcpy.strcpy_errno_clobbered
 */
#include "pc.h"
#include <errno.h>
#include <string.h>

int main(void)
{
	static char src[] = "posixcheck", dst[64];
	static const int before[2] = { 0, EDOM };
	char *volatile end;	/* used, so the call stays a stpcpy() call */
	int i, e;

	pc_begin("XSH.stpcpy.003");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		end = stpcpy(dst, src);
		e = errno;
		pc_check(e == before[i], "stpcpy() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)strcpy(dst, src);
		e = errno;
		pc_check(e == before[i], "strcpy() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	(void)end;
	pc_pass();
}
