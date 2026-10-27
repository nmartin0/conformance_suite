/*
 * When no template line matches the input getdate() fails with getdate_err 7.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.005
 */
#include "gd.h"

int main(void)
{
	static const char *const bad[] = { "hello", "11-27-86", "27 November", "ab/cd/ef", "011/27/86", "11/271/86" };
	size_t i;

	pc_begin("XSH.getdate.005");
	gd_setup("%m/%d/%y\n%A\n");
	pc_test();
	for (i = 0; i < GD_N(bad); i++) {
		struct tm *p = getdate(bad[i]);

		pc_check(p == NULL, "getdate(\"%s\") returned a pointer", bad[i]);
		pc_check(getdate_err == 7, "getdate(\"%s\"): getdate_err is %d, expected 7", bad[i],
		    (int)getdate_err);
	}
	pc_pass();
}
