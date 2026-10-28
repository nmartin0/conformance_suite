/*
 * When TZ names only a standard time, daylight saving time does not
 * apply: conversions in every month use the same offset and tm_isdst is
 * 0, whatever the date.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.3-other-environment-variables.003
 */
#include "tzh.h"

int main(void)
{
	static const char *const tzs[] = { "PCT5", "<PCQ>-1", "PCU3:30", "PCV-9" };
	static const long east[] = { -18000, 3600, -12600, 32400 };
	size_t i;
	int y, m;

	pc_begin("XBD.8.3-other-environment-variables.003");
	pc_test();
	for (i = 0; i < sizeof tzs / sizeof tzs[0]; i++) {
		tz_set(tzs[i]);
		for (y = 2021; y <= 2024; y += 3)
			for (m = 1; m <= 12; m++)
				tz_expect(tzs[i], tz_utc(y, m, 15, 12, 0, 0), east[i], 0, "mid-month");
	}
	pc_pass();
}
