/*
 * An input that matches a template but names an impossible date gives getdate_err 8.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.006
 */
#include "gd.h"

int main(void)
{
	static const char *const bad[] = { "02/31/20", "02/30/04", "04/31/21", "02/29/21", "06/31/10" };
	size_t i;

	pc_begin("XSH.getdate.006");
	gd_setup("%m/%d/%y\n");
	pc_test();
	for (i = 0; i < GD_N(bad); i++) {
		struct tm *p = getdate(bad[i]);

		pc_check(p == NULL, "getdate(\"%s\") returned a pointer", bad[i]);
		pc_check(getdate_err == 8, "getdate(\"%s\"): getdate_err is %d, expected 8", bad[i],
		    (int)getdate_err);
	}
	pc_pass();
}
