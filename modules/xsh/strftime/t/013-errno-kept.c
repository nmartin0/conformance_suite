/*
 * A successful strftime() does not change errno.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strftime.013
 * MUTANT    strftime.errno_zero
 * MUTANT    strftime.errno_erange
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ALL "%a %A %b %B %c %C %d %D %e %F %g %G %h %H %I %j %m %M %n %p %r %R %s %S %t %T %u %U %V %w %W %x %X %y %Y %z %Z %%"

int main(void)
{
	time_t t = 1626364800;	/* 2021-07-15 16:00:00 UTC */
	struct tm tm, *p;
	char buf[512];
	size_t n;
	int e;

	pc_begin("XSH.strftime.013");
	if (setenv("TZ", "PCS5PCD,M3.2.0,M11.1.0", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	p = localtime(&t);
	pc_check(p != NULL, "localtime() failed");
	tm = *p;
	pc_test();
	errno = ENOTTY;
	n = strftime(buf, sizeof buf, ALL, &tm);
	e = errno;
	pc_check(n > 0 && n < sizeof buf, "strftime() of every specifier returned %zu", n);
	pc_check(e == ENOTTY, "a successful strftime() changed errno from ENOTTY to %s", pc_errname(e));
	errno = EDOM;
	n = strftime(buf, sizeof buf, "plain text", &tm);
	e = errno;
	pc_check(n == 10, "strftime(\"plain text\") returned %zu", n);
	pc_check(e == EDOM, "a successful strftime() changed errno from EDOM to %s", pc_errname(e));
	pc_pass();
}
