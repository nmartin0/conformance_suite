/*
 * strcat: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcat.006
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile f_strcat)(char *restrict, const char *restrict) = strcat;

int main(void)
{
	int err;
	char d[16];
	pc_begin("XSH.strcat.006");
	pc_test();
	errno = EDOM;
	d[0] = 0; (void)f_strcat(d, "abc");
	err = errno;
	pc_check(err == EDOM, "strcat() changed errno to %s", pc_errname(err));
	pc_pass();
}
