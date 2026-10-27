/*
 * A template file that does not exist gives getdate_err 2 (cannot be opened) or 3 (no file status).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.003
 */
#include "gd.h"

int main(void)
{
	char path[640];
	struct tm *p;

	pc_begin("XSH.getdate.003");
	gd_path("pcg.nonexistent", path, sizeof path);
	pc_check(access(path, F_OK) != 0, "the file exists");
	pc_check(setenv("DATEMSK", path, 1) == 0, "setenv failed");
	pc_test();
	p = getdate("11/27/86");
	pc_check(p == NULL, "getdate returned a pointer");
	pc_check(getdate_err == 2 || getdate_err == 3, "getdate_err is %d, expected 2 or 3", (int)getdate_err);
	pc_pass();
}
