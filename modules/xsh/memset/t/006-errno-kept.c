/*
 * memset: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memset.006
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static void *(*volatile f_memset)(void *, int, size_t) = memset;

int main(void)
{
	int err;
	char b[4];
	pc_begin("XSH.memset.006");
	pc_test();
	errno = EDOM;
	(void)f_memset(b, 1, 4);
	err = errno;
	pc_check(err == EDOM, "memset() changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_memset(b, 1, 0);
	err = errno;
	pc_check(err == EDOM, "memset() with n 0 changed errno to %s", pc_errname(err));
	pc_pass();
}
