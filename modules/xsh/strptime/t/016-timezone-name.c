/*
 * %Z sets tm_isdst to 1 for the daylight name in tzname[1] and to 0 for the standard name in tzname[0].
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.016
 */
#include "sp.h"

int main(void)
{
	pc_begin("XSH.strptime.016");
	if (setenv("TZ", "PCA5PCB", 1) != 0)
		pc_unresolved("setenv TZ failed");
	tzset();
	if (strcmp(tzname[0], "PCA") != 0 || strcmp(tzname[1], "PCB") != 0)
		pc_unresolved("tzset did not set tzname from the TZ value (got \"%s\" and \"%s\")", tzname[0], tzname[1]);
	pc_test();
	sp_case("PCB", "%Z", 3, SP_ISDST, (struct tm){ .tm_isdst = 1 });
	sp_case("PCA", "%Z", 3, SP_ISDST, (struct tm){ .tm_isdst = 0 });
	sp_case("PCB,x", "%Z,", 4, SP_ISDST, (struct tm){ .tm_isdst = 1 });
	sp_case("PCA,x", "%Z,", 4, SP_ISDST, (struct tm){ .tm_isdst = 0 });
	sp_case("12:00 PCB", "%R %Z", 9, SP_HOUR | SP_ISDST, (struct tm){ .tm_hour = 12, .tm_isdst = 1 });
	pc_pass();
}
