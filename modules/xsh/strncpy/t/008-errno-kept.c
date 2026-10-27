/*
 * strncpy: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncpy.008
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile f_strncpy)(char *restrict, const char *restrict, size_t) = strncpy;
static char *(*volatile f_stpncpy)(char *restrict, const char *restrict, size_t) = stpncpy;

int main(void)
{
	int err;
	char d[8];
	pc_begin("XSH.strncpy.008");
	pc_test();
	errno = EDOM;
	(void)f_strncpy(d, "ab", 8);
	err = errno;
	pc_check(err == EDOM, "strncpy() changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_stpncpy(d, "ab", 8);
	err = errno;
	pc_check(err == EDOM, "stpncpy() changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strncpy(d, "abcdefghij", 8);
	err = errno;
	pc_check(err == EDOM, "strncpy() with truncation changed errno to %s", pc_errname(err));
	pc_pass();
}
