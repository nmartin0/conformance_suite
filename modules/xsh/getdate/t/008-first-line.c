/*
 * The first template line that matches the input is the one used.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.008
 */
#include "gd.h"

int main(void)
{
	struct tm r;

	pc_begin("XSH.getdate.008");
	gd_setup("%d/%m/%y\n%m/%d/%y\n");
	pc_test();
	r = gd_get("10/11/12");
	gd_fields(&r, "10/11/12 with day first", 112, 10, 10, -1, -1, -1);
	pc_check(setenv("TZ", "UTC0", 1) == 0, "setenv failed");
	gd_setup("%A\n%m/%d/%y\n");
	r = gd_get("11/27/86");
	gd_fields(&r, "11/27/86 after a non-matching line", 86, 10, 27, -1, -1, -1);
	gd_setup("%m/%d/%y\n%d/%m/%y\n");
	r = gd_get("10/11/12");
	gd_fields(&r, "10/11/12 with month first", 112, 9, 11, -1, -1, -1);
	pc_pass();
}
