/*
 * tzset() sets tzname from the std and dst parts of TZ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tzset.001
 * MUTANT    tzset.noop
 * MUTANT    tzset.swap
 * MUTANT    tzset.trunc3
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void names(const char *tz, const char *n0, const char *n1)
{
	if (setenv("TZ", tz, 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	pc_check(tzname[0] != NULL && strcmp(tzname[0], n0) == 0, "TZ=%s: tzname[0] is \"%s\", expected \"%s\"", tz,
	    tzname[0] ? tzname[0] : "(null)", n0);
	if (n1 != NULL)
		pc_check(tzname[1] != NULL && strcmp(tzname[1], n1) == 0, "TZ=%s: tzname[1] is \"%s\", expected \"%s\"",
		    tz, tzname[1] ? tzname[1] : "(null)", n1);
}

int main(void)
{
	pc_begin("XSH.tzset.001");
	pc_test();
	names("PCS5PCD,M3.2.0,M11.1.0", "PCS", "PCD");
	names("ABCDEF5GHIJKL,M3.2.0,M11.1.0", "ABCDEF", "GHIJKL");
	names("PCT3", "PCT", NULL);	/* no dst: tzname[1] is not specified */
	names("PCS5PCD,M3.2.0,M11.1.0", "PCS", "PCD");
	pc_pass();
}
