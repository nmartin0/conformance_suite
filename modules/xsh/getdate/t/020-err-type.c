/*
 * getdate_err is an int that <time.h> declares, a success returns a struct tm within range.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.020
 */
#include "gd.h"

int main(void)
{
	int e;
	struct tm *p;

	pc_begin("XSH.getdate.020");
	pc_check(unsetenv("DATEMSK") == 0, "unsetenv failed");
	pc_test();
	pc_check(_Generic(getdate_err, int: 1, default: 0), "getdate_err is not an int");
	p = getdate("11/27/86");
	pc_check(p == NULL, "getdate returned a pointer without DATEMSK");
	e = getdate_err;
	pc_check(e == 1, "getdate_err is %d, expected 1", e);
	gd_setup("%m/%d/%y\n");
	p = getdate("11/27/86");
	pc_check(p != NULL, "getdate failed with getdate_err %d", (int)getdate_err);
	pc_check(p->tm_sec >= 0 && p->tm_sec <= 60 && p->tm_min >= 0 && p->tm_min <= 59 &&
	    p->tm_hour >= 0 && p->tm_hour <= 23 && p->tm_mday >= 1 && p->tm_mday <= 31 &&
	    p->tm_mon >= 0 && p->tm_mon <= 11 && p->tm_wday >= 0 && p->tm_wday <= 6 &&
	    p->tm_yday >= 0 && p->tm_yday <= 365, "the broken-down time has a member out of range");
	pc_pass();
}
