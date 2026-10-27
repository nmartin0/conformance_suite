/*
 * strchr: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strchr.005
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile f_strchr)(const char *, int) = strchr;

int main(void)
{
	int err;
	static const char s[] = "abc";
	pc_begin("XSH.strchr.005");
	pc_test();
	errno = EDOM;
	(void)f_strchr(s, 'b');
	err = errno;
	pc_check(err == EDOM, "strchr() finding a byte changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strchr(s, 'z');
	err = errno;
	pc_check(err == EDOM, "strchr() not finding a byte changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strchr(s, 0);
	err = errno;
	pc_check(err == EDOM, "strchr() for the terminator changed errno to %s", pc_errname(err));
	pc_pass();
}
