/*
 * memccpy() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memccpy.004
 * MUTANT    memccpy.errno_clobbered
 * MUTANT    memccpy.errno_not_found
 */
#include "pc.h"
#include <errno.h>
#include <string.h>

static char src[] = "posixcheck", dst[32];

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.memccpy.004");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)memccpy(dst, src, 'x', sizeof src);
		e = errno;
		pc_check(e == before[i], "memccpy() with c found changed errno from %s to %s",
		    pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)memccpy(dst, src, 'z', sizeof src);
		e = errno;
		pc_check(e == before[i], "memccpy() with c not found changed errno from %s to %s",
		    pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
