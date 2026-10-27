/*
 * struct tm members from gmtime and localtime are in range.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.005
 */
#include <time.h>
#include "pc.h"

static void check(const char *who, time_t t, const struct tm *p)
{
	pc_check(p->tm_sec >= 0 && p->tm_sec <= 60, "%s(%lld): tm_sec is %d", who, (long long)t, p->tm_sec);
	pc_check(p->tm_min >= 0 && p->tm_min <= 59, "%s(%lld): tm_min is %d", who, (long long)t, p->tm_min);
	pc_check(p->tm_hour >= 0 && p->tm_hour <= 23, "%s(%lld): tm_hour is %d", who, (long long)t, p->tm_hour);
	pc_check(p->tm_mday >= 1 && p->tm_mday <= 31, "%s(%lld): tm_mday is %d", who, (long long)t, p->tm_mday);
	pc_check(p->tm_mon >= 0 && p->tm_mon <= 11, "%s(%lld): tm_mon is %d", who, (long long)t, p->tm_mon);
	pc_check(p->tm_wday >= 0 && p->tm_wday <= 6, "%s(%lld): tm_wday is %d", who, (long long)t, p->tm_wday);
	pc_check(p->tm_yday >= 0 && p->tm_yday <= 365, "%s(%lld): tm_yday is %d", who, (long long)t, p->tm_yday);
	pc_check(p->tm_isdst >= 0, "%s(%lld): tm_isdst is negative (%d)", who, (long long)t, p->tm_isdst);
}

int main(void)
{
	pc_begin("XBD.time.h.005");
	pc_test();
	static const long long v[] = { 0, 1, 59, 3600, 86399, 86400, 951782400LL, 1000000000LL, 1234567890LL, 1700000000LL,
	    2000000000LL, 4102444799LL, -1, -86400, -86400LL * 365 * 30 };
	size_t i;

	for (i = 0; i < sizeof v / sizeof v[0]; i++) {
		time_t t = (time_t)v[i];
		struct tm r, *p;

		p = gmtime_r(&t, &r);
		if (p != NULL)
			check("gmtime_r", t, p);
		p = gmtime(&t);
		if (p != NULL)
			check("gmtime", t, p);
		p = localtime_r(&t, &r);
		if (p != NULL)
			check("localtime_r", t, p);
		p = localtime(&t);
		if (p != NULL)
			check("localtime", t, p);
	}
	pc_pass();
}
