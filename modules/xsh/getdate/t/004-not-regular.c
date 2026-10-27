/*
 * A template file that is not a regular file gives getdate_err 4.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.004
 */
#include "gd.h"

int main(void)
{
	char path[640];
	struct tm *p;

	pc_begin("XSH.getdate.004");
	pc_make_dir("pcg.dir");
	gd_path("pcg.dir", path, sizeof path);
	pc_check(setenv("DATEMSK", path, 1) == 0, "setenv failed");
	pc_test();
	p = getdate("11/27/86");
	pc_check(p == NULL, "getdate returned a pointer");
	pc_check(getdate_err == 4, "getdate_err is %d, expected 4", (int)getdate_err);
	(void)rmdir("pcg.dir");
	pc_pass();
}
