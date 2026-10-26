/*
 * localtime_r() sets tzname, daylight and timezone all together or not at all.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localtime.009
 * MUTANT    localtime.r_tzname_only
 * MUTANT    localtime.r_daylight_only
 * VARIANT   localtime.r_sets_tz
 * VARIANT   localtime.r_keeps_tz
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
	time_t t = 1234567890;
	struct tm buf;
	char n0[64], n1[64];
	int d0, old, new;
	long z0;

	pc_begin("XSH.localtime.009");
	if (setenv("TZ", "PCA3", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	pc_check(tzname[0] != NULL && strcmp(tzname[0], "PCA") == 0 && daylight == 0 && timezone == 10800,
	    "tzset() did not set up TZ=PCA3 (tzname[0] \"%s\", daylight %d, timezone %ld)",
	    tzname[0] ? tzname[0] : "(null)", daylight, timezone);
	snprintf(n0, sizeof n0, "%s", tzname[0]);
	snprintf(n1, sizeof n1, "%s", tzname[1] ? tzname[1] : "");
	d0 = daylight;
	z0 = timezone;
	if (setenv("TZ", "PCB-2PCC,M3.5.0,M10.5.0/3", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	pc_test();
	pc_check(localtime_r(&t, &buf) == &buf, "localtime_r() failed: %s", pc_errname(errno));
	old = strcmp(tzname[0] ? tzname[0] : "", n0) == 0 && strcmp(tzname[1] ? tzname[1] : "", n1) == 0 &&
	    daylight == d0 && timezone == z0;
	new = tzname[0] != NULL && strcmp(tzname[0], "PCB") == 0 && tzname[1] != NULL &&
	    strcmp(tzname[1], "PCC") == 0 && daylight != 0 && timezone == -7200;
	pc_check(old || new,
	    "after localtime_r() tzname is \"%s\" \"%s\", daylight %d, timezone %ld: neither all of TZ=PCA3 nor all of the new TZ",
	    tzname[0] ? tzname[0] : "(null)", tzname[1] ? tzname[1] : "(null)", daylight, timezone);
	pc_note("localtime_r() %s the timezone information", new ? "set" : "did not set");
	pc_pass();
}
