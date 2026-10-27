/*
 * strlcpy: copy errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.007
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	int err;
	char d[8];
	pc_begin("XSH.strlcpy.007");
	pc_test();
	errno = EDOM;
	(void)f_strlcpy(d, "abc", 8);
	err = errno;
	pc_check(err == EDOM, "strlcpy() changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strlcpy(d, "abcdefghij", 8);
	err = errno;
	pc_check(err == EDOM, "strlcpy() with truncation changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strlcpy(d, "abc", 0);
	err = errno;
	pc_check(err == EDOM, "strlcpy() with dstsize 0 changed errno to %s", pc_errname(err));
	pc_pass();
}
