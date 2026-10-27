/*
 * With DATEMSK set to an empty string getdate() fails with getdate_err 1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.002
 */
#include "gd.h"

int main(void)
{
	struct tm *p;

	pc_begin("XSH.getdate.002");
	pc_check(setenv("DATEMSK", "", 1) == 0, "setenv failed");
	pc_test();
	p = getdate("11/27/86");
	pc_check(p == NULL, "getdate returned a pointer");
	pc_check(getdate_err == 1, "getdate_err is %d, expected 1", (int)getdate_err);
	pc_pass();
}
