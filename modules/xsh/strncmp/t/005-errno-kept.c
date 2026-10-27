/*
 * strncmp: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncmp.005
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static int (*volatile f_strncmp)(const char *, const char *, size_t) = strncmp;

int main(void)
{
	int err;

	pc_begin("XSH.strncmp.005");
	pc_test();
	errno = EDOM;
	(void)f_strncmp("abc", "abc", 3);
	err = errno;
	pc_check(err == EDOM, "strncmp() of equal strings changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strncmp("abc", "abd", 3);
	err = errno;
	pc_check(err == EDOM, "strncmp() of different strings changed errno to %s", pc_errname(err));
	pc_pass();
}
