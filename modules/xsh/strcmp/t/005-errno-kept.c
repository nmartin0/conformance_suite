/*
 * strcmp: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcmp.005
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static int (*volatile f_strcmp)(const char *, const char *) = strcmp;

int main(void)
{
	int err;

	pc_begin("XSH.strcmp.005");
	pc_test();
	errno = EDOM;
	(void)f_strcmp("abc", "abc");
	err = errno;
	pc_check(err == EDOM, "strcmp() of equal strings changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strcmp("abc", "abd");
	err = errno;
	pc_check(err == EDOM, "strcmp() of different strings changed errno to %s", pc_errname(err));
	pc_pass();
}
