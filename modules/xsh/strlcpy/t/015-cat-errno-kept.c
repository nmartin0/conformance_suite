/*
 * strlcpy: cat errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.015
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
	pc_begin("XSH.strlcpy.015");
	pc_test();
	errno = EDOM;
	strcpy(d, "ab"); (void)f_strlcat(d, "cd", 8);
	err = errno;
	pc_check(err == EDOM, "strlcat() changed errno to %s", pc_errname(err));
	errno = EDOM;
	strcpy(d, "ab"); (void)f_strlcat(d, "cdefghij", 8);
	err = errno;
	pc_check(err == EDOM, "strlcat() with truncation changed errno to %s", pc_errname(err));
	errno = EDOM;
	strcpy(d, "ab"); (void)f_strlcat(d, "cd", 0);
	err = errno;
	pc_check(err == EDOM, "strlcat() with dstsize 0 changed errno to %s", pc_errname(err));
	pc_pass();
}
