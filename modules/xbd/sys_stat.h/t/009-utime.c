/*
 * UTIME_NOW and UTIME_OMIT are different, lie outside 0 to 999999999 and
 * survive being stored in tv_nsec.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.009
 */
#include "pc.h"
#include <sys/stat.h>

int main(void)
{
	pc_begin("XBD.sys_stat.h.009");
#if !defined(UTIME_NOW) || !defined(UTIME_OMIT)
	pc_unresolved("UTIME_NOW or UTIME_OMIT is missing (the header check reports it)");
#else
	long long now = (long long)(UTIME_NOW), omit = (long long)(UTIME_OMIT);

	pc_test();
	pc_check(now != omit, "UTIME_NOW and UTIME_OMIT are both %lld", now);
	pc_check(now < 0 || now > 999999999, "UTIME_NOW (%lld) is a valid nanoseconds value", now);
	pc_check(omit < 0 || omit > 999999999, "UTIME_OMIT (%lld) is a valid nanoseconds value", omit);
	/* They go into tv_nsec, so they must fit that member. */
	{
		struct timespec ts;

		ts.tv_nsec = UTIME_NOW;
		pc_check((long long)ts.tv_nsec == now, "UTIME_NOW does not survive being stored in tv_nsec");
		ts.tv_nsec = UTIME_OMIT;
		pc_check((long long)ts.tv_nsec == omit, "UTIME_OMIT does not survive being stored in tv_nsec");
	}
	pc_pass();
#endif
}
