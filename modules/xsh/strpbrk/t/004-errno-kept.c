/*
 * strpbrk: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strpbrk.004
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile f_strpbrk)(const char *, const char *) = strpbrk;

int main(void)
{
	int err;

	pc_begin("XSH.strpbrk.004");
	pc_test();
	errno = EDOM;
	(void)f_strpbrk("abc", "b");
	err = errno;
	pc_check(err == EDOM, "strpbrk() finding a byte changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strpbrk("abc", "z");
	err = errno;
	pc_check(err == EDOM, "strpbrk() finding none changed errno to %s", pc_errname(err));
	pc_pass();
}
