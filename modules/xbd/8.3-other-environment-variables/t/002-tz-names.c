/*
 * The standard-time and daylight-saving names in TZ are taken from the
 * unquoted form (letters) or the quoted form (angle brackets around
 * letters, digits, plus and minus, which do not count in the name), and
 * tzname[] gives them without the quoting characters; the offset that
 * follows a quoted name is read as usual.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.3-other-environment-variables.002
 */
#include "tzh.h"

int main(void)
{
	static const struct { const char *tz; const char *std; const char *dst; long east; } c[] = {
		{ "PCT3", "PCT", NULL, -10800 },
		{ "<AB1>2", "AB1", NULL, -7200 },
		{ "<+03>-3", "+03", NULL, 10800 },
		{ "<-03>3", "-03", NULL, -10800 },
		{ "<A-B>3", "A-B", NULL, -10800 },
		{ "<ABCDEF>5", "ABCDEF", NULL, -18000 },
		{ "ABCDEF5", "ABCDEF", NULL, -18000 },
		{ "<AB1>2<CD+2>1,M3.2.0,M11.1.0", "AB1", "CD+2", -7200 },
		{ "PCS5PCD,M3.2.0,M11.1.0", "PCS", "PCD", -18000 },
		{ "<PCS>5<PCD>,M3.2.0,M11.1.0", "PCS", "PCD", -18000 },
	};
	size_t i;
	long long t;

	pc_begin("XBD.8.3-other-environment-variables.002");
	t = tz_utc(2021, 1, 15, 12, 0, 0);
	pc_test();
	for (i = 0; i < sizeof c / sizeof c[0]; i++) {
		tz_set(c[i].tz);
		pc_check(strcmp(tzname[0], c[i].std) == 0, "TZ=%s: tzname[0] is \"%s\", not \"%s\"", c[i].tz, tzname[0], c[i].std);
		if (c[i].dst != NULL)
			pc_check(strcmp(tzname[1], c[i].dst) == 0, "TZ=%s: tzname[1] is \"%s\", not \"%s\"", c[i].tz, tzname[1], c[i].dst);
		tz_expect(c[i].tz, t, c[i].east, 0, "January");
	}
	pc_pass();
}
