/*
 * The Mm.n.d date form of a TZ rule names the d'th weekday (0 is Sunday)
 * of week n of month m, week 5 meaning the last such day of the month even
 * when the month has only four: changes fall on the second Friday of
 * March and first Tuesday of November, and on the last Sunday of February
 * (a month whose four Sundays make week 5 the fourth) and last Saturday
 * of October (five Saturdays), at 02:00 local time of the time in effect.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.3-other-environment-variables.004
 */
#include "tzh.h"

#define W -18000
#define S -14400

static void change(const char *tz, long long t, int before_dst, const char *what)
{
	tz_expect(tz, t - 1, before_dst ? S : W, before_dst, what);
	tz_expect(tz, t, before_dst ? W : S, !before_dst, what);
}

int main(void)
{
	const char *a = "PCS5PCD,M3.2.5,M11.1.2", *b = "PCS5PCD,M2.5.0,M10.5.6";

	pc_begin("XBD.8.3-other-environment-variables.004");
	pc_test();
	tz_set(a);
	/* 2021: Friday 12 March, Tuesday 2 November */
	change(a, tz_utc(2021, 3, 12, 7, 0, 0), 0, "start on the second Friday of March 2021");
	change(a, tz_utc(2021, 11, 2, 6, 0, 0), 1, "end on the first Tuesday of November 2021");
	tz_set(b);
	/* 2021: Sunday 28 February (the fourth Sunday), Saturday 30 October (the fifth Saturday) */
	change(b, tz_utc(2021, 2, 28, 7, 0, 0), 0, "start on the last Sunday of February 2021");
	change(b, tz_utc(2021, 10, 30, 6, 0, 0), 1, "end on the last Saturday of October 2021");
	/* 2024 (leap year): Sunday 25 February (the fourth of four), Saturday 26 October (the fourth of four) */
	change(b, tz_utc(2024, 2, 25, 7, 0, 0), 0, "start on the last Sunday of February 2024");
	change(b, tz_utc(2024, 10, 26, 6, 0, 0), 1, "end on the last Saturday of October 2024");
	pc_pass();
}
