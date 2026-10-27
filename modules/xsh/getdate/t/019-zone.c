/*
 * %Z accepts the time zone name that getdate() expects and rejects another with getdate_err 8.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.019
 */
#include "gd.h"

int main(void)
{
	struct tm r, *p;

	pc_begin("XSH.getdate.019");
	gd_setup("%H:%M %Z\n");
	pc_test();
	r = gd_get("10:30 UTC");
	gd_fields(&r, "10:30 UTC", -1, -1, -1, 10, 30, 0);
	p = getdate("10:30 ZZQ");
	pc_check(p == NULL, "getdate with an unknown zone name returned a pointer");
	pc_check(getdate_err == 7 || getdate_err == 8, "getdate_err is %d, expected 7 or 8", (int)getdate_err);
	pc_pass();
}
