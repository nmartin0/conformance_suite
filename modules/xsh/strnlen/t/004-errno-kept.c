/*
 * strlen() and strnlen() leave errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strnlen.004
 * MUTANT    strnlen.errno_clobbered
 * MUTANT    strnlen.strlen_errno_clobbered
 */
#include "pc.h"
#include <errno.h>
#include <string.h>

static char s[] = "posixcheck";

int main(void)
{
	static const int before[2] = { 0, EDOM };
	volatile size_t sink;
	int i, e;

	pc_begin("XSH.strnlen.004");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		sink = strlen(s);
		e = errno;
		pc_check(e == before[i], "strlen() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		sink = strnlen(s, 4);
		e = errno;
		pc_check(e == before[i], "strnlen() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	(void)sink;
	pc_pass();
}
