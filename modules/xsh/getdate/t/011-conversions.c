/*
 * The conversions %D %T %R %e %h %n %t and %% work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.011
 */
#include "gd.h"

static const struct { const char *tmpl, *in; int y, m, d, H, M, S; } tab[] = {
	{ "%D", "11/27/86", 86, 10, 27, -1, -1, -1 },
	{ "%m/%d/%Y %T", "01/02/2003 04:05:06", 103, 0, 2, 4, 5, 6 },
	{ "%d %h %Y %R", "5 Mar 2001 07:08", 101, 2, 5, 7, 8, 0 },
	{ "%e %b %Y", "07 jan 1999", 99, 0, 7, -1, -1, -1 },
	{ "%Y%n%m%t%d", "2001\n02\t03", 101, 1, 3, -1, -1, -1 },
	{ "%d%%%m", "10%11", -1, 10, 10, -1, -1, -1 },
	{ "%Y %B %d", "1999 december 31", 99, 11, 31, -1, -1, -1 },
	{ "%d/%m/%Y %H.%M.%S", "05/06/2007 08.09.10", 107, 5, 5, 8, 9, 10 },
	{ "%B %Y", "March 2001", 101, 2, -1, -1, -1, -1 },
};

int main(void)
{
	size_t i;
	char line[64];

	pc_begin("XSH.getdate.011");
	gd_setup("%D\n");
	pc_test();
	for (i = 0; i < GD_N(tab); i++) {
		struct tm r;

		snprintf(line, sizeof line, "%s\n", tab[i].tmpl);
		gd_setup(line);
		r = gd_get(tab[i].in);
		gd_fields(&r, tab[i].in, tab[i].y, tab[i].m, tab[i].d, tab[i].H, tab[i].M, tab[i].S);
	}
	pc_pass();
}
