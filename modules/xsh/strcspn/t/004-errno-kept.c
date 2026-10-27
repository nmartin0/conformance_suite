/*
 * strcspn: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcspn.004
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static size_t (*volatile f_strcspn)(const char *, const char *) = strcspn;

int main(void)
{
	int err;

	pc_begin("XSH.strcspn.004");
	pc_test();
	errno = EDOM;
	(void)f_strcspn("abc", "b");
	err = errno;
	pc_check(err == EDOM, "strcspn() changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strcspn("abc", "z");
	err = errno;
	pc_check(err == EDOM, "strcspn() with no member changed errno to %s", pc_errname(err));
	pc_pass();
}
