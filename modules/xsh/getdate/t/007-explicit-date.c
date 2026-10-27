/*
 * A date without a time of day takes the current hour, minute and second.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.007
 */
#include "gd.h"

int main(void)
{
	static const struct { const char *in; int y, m, d; } tab[] = {
		{ "11/27/86", 86, 10, 27 }, { "1/2/03", 103, 0, 2 }, { "12/31/99", 99, 11, 31 },
		{ "02/29/04", 104, 1, 29 },
	};
	size_t i;

	pc_begin("XSH.getdate.007");
	gd_setup("%m/%d/%y\n");
	pc_test();
	for (i = 0; i < GD_N(tab); i++) {
		time_t t1 = time(NULL), t2;
		struct tm r = gd_get(tab[i].in);

		t2 = time(NULL);
		gd_fields(&r, tab[i].in, tab[i].y, tab[i].m, tab[i].d, -1, -1, -1);
		pc_check(gd_hms_now(&r, t1, t2), "%s: the time of day %02d:%02d:%02d is not the current one",
		    tab[i].in, r.tm_hour, r.tm_min, r.tm_sec);
	}
	pc_pass();
}
