/*
 * With %C a century and %y the year in it give the year.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.013
 */
#include "gd.h"

int main(void)
{
	struct tm r;

	pc_begin("XSH.getdate.013");
	gd_setup("%C %y %m %d\n");
	pc_test();
	r = gd_get("20 05 06 07");
	gd_fields(&r, "century 20, year 05", 105, 5, 7, -1, -1, -1);
	r = gd_get("19 05 06 07");
	gd_fields(&r, "century 19, year 05", 5, 5, 7, -1, -1, -1);
	r = gd_get("19 86 11 27");
	gd_fields(&r, "century 19, year 86", 86, 10, 27, -1, -1, -1);
	pc_pass();
}
