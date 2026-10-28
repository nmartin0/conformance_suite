/*
 * The offset after the standard-time name in TZ is hours with optional
 * minutes and seconds, added to local time to give UTC: a plain or
 * plus-signed value is west of the Prime Meridian and a minus-signed
 * value east, a leading zero and a single digit read alike, and zero and
 * 24 hours are accepted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.3-other-environment-variables.001
 */
#include "tzh.h"

int main(void)
{
	static const struct { const char *tz; long east; } c[] = {
		{ "PCT3", -10800 }, { "PCT+3", -10800 }, { "PCT-3", 10800 }, { "PCT03", -10800 }, { "PCT003", -10800 },
		{ "PCT5:30", -19800 }, { "PCT-5:30", 19800 }, { "PCT+5:30", -19800 }, { "PCT4:15:30", -15330 },
		{ "PCT-4:15:30", 15330 }, { "PCT0", 0 }, { "PCT24", -86400 }, { "PCT-24", 86400 }, { "PCT-0:30", 1800 },
		{ "PCT12", -43200 }, { "PCT-12:00:01", 43201 },
	};
	size_t i;
	long long t1, t2;

	pc_begin("XBD.8.3-other-environment-variables.001");
	t1 = tz_utc(2021, 1, 15, 12, 0, 0);
	t2 = tz_utc(2021, 7, 15, 12, 0, 0);
	pc_test();
	for (i = 0; i < sizeof c / sizeof c[0]; i++) {
		tz_set(c[i].tz);
		tz_expect(c[i].tz, t1, c[i].east, 0, "January");
		tz_expect(c[i].tz, t2, c[i].east, 0, "July");
	}
	pc_pass();
}
