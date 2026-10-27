/*
 * strstr: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strstr.005
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile f_strstr)(const char *, const char *) = strstr;

int main(void)
{
	int err;

	pc_begin("XSH.strstr.005");
	pc_test();
	errno = EDOM;
	(void)f_strstr("abc", "bc");
	err = errno;
	pc_check(err == EDOM, "strstr() finding a string changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strstr("abc", "z");
	err = errno;
	pc_check(err == EDOM, "strstr() finding none changed errno to %s", pc_errname(err));
	pc_pass();
}
