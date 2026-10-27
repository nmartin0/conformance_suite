/*
 * strspn: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strspn.004
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static size_t (*volatile f_strspn)(const char *, const char *) = strspn;

int main(void)
{
	int err;

	pc_begin("XSH.strspn.004");
	pc_test();
	errno = EDOM;
	(void)f_strspn("abc", "ab");
	err = errno;
	pc_check(err == EDOM, "strspn() changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strspn("abc", "z");
	err = errno;
	pc_check(err == EDOM, "strspn() with no member changed errno to %s", pc_errname(err));
	pc_pass();
}
