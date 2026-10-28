/*
 * A rule time in TZ may carry minutes and seconds (hh:mm:ss) and be as
 * large as 167 hours: DST starts at 01:30:15 standard time and ends at
 * 00:45:30 daylight time under such times, and a start time of 167 hours
 * puts the change a day short of a week after midnight of the date.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.3-other-environment-variables.005
 */
#include "tzh.h"

#define W -18000
#define S -14400

int main(void)
{
	const char *a = "PCS5PCD,M3.2.0/1:30:15,M11.1.0/0:45:30", *b = "PCS5PCD,M3.2.0/167,M11.1.0";
	long long t;

	pc_begin("XBD.8.3-other-environment-variables.005");
	pc_test();
	tz_set(a);
	t = tz_utc(2021, 3, 14, 6, 30, 15); /* 01:30:15 standard time, 14 March 2021 (second Sunday) */
	tz_expect(a, t - 1, W, 0, "just before the start");
	tz_expect(a, t, S, 1, "at the start");
	t = tz_utc(2021, 11, 7, 4, 45, 30); /* 00:45:30 daylight time, 7 November 2021 (first Sunday) */
	tz_expect(a, t - 1, S, 1, "just before the end");
	tz_expect(a, t, W, 0, "at the end");
	tz_set(b);
	/* 167 hours after midnight of Sunday 14 March is 23:00 on Saturday 20 March, standard time */
	t = tz_utc(2021, 3, 21, 4, 0, 0);
	tz_expect(b, t - 1, W, 0, "just before a start 167 hours after midnight");
	tz_expect(b, t, S, 1, "at a start 167 hours after midnight");
	pc_pass();
}
