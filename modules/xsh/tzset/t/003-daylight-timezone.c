/*
 * tzset() sets daylight and timezone (XSI).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tzset.003
 * MUTANT    tzset.daylight_always
 * MUTANT    tzset.timezone_sign
 * MUTANT    tzset.timezone_dst
 * MUTANT    tzset.timezone_no_secs
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void zone(const char *tz, int dst, long west)
{
	if (setenv("TZ", tz, 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	if (dst)
		pc_check(daylight != 0, "TZ=%s: daylight is 0", tz);
	else
		pc_check(daylight == 0, "TZ=%s: daylight is %d, expected 0", tz, daylight);
	pc_check(timezone == west, "TZ=%s: timezone is %ld, expected %ld", tz, timezone, west);
}

int main(void)
{
	pc_begin("XSH.tzset.003");
	pc_test();
	zone("PCT3", 0, 10800);
	zone("PCE-5:30", 0, -19800);
	zone("PCP+4:15:30", 0, 15330);
	zone("PCS5PCD,M3.2.0,M11.1.0", 1, 18000);
	pc_pass();
}
