/*
 * strrchr: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strrchr.005
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile f_strrchr)(const char *, int) = strrchr;

int main(void)
{
	int err;
	static const char s[] = "abc";
	pc_begin("XSH.strrchr.005");
	pc_test();
	errno = EDOM;
	(void)f_strrchr(s, 'b');
	err = errno;
	pc_check(err == EDOM, "strrchr() finding a byte changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strrchr(s, 'z');
	err = errno;
	pc_check(err == EDOM, "strrchr() not finding a byte changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_strrchr(s, 0);
	err = errno;
	pc_check(err == EDOM, "strrchr() for the terminator changed errno to %s", pc_errname(err));
	pc_pass();
}
