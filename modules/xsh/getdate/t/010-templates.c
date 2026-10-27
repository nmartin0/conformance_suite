/*
 * Full date and time templates convert, case-insensitively and with optional leading zeros.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.010
 */
#include "gd.h"

static const struct { const char *in; int y, m, d, H, M, S; } tab[] = {
	{ "Friday September 18, 1987, 10:30:30", 87, 8, 18, 10, 30, 30 },
	{ "24,9,1986 10:30", 86, 8, 24, 10, 30, 0 },
	{ "fRiDaY sEpTeMbEr 18, 1987, 01:02:03", 87, 8, 18, 1, 2, 3 },
	{ "FRIDAY SEPTEMBER 18, 1987, 01:02:03", 87, 8, 18, 1, 2, 3 },
	{ "9,1,2001 7:05", 101, 0, 9, 7, 5, 0 },
	{ "09,01,2001 07:05", 101, 0, 9, 7, 5, 0 },
	{ "friday september 4, 1987, 1:2:3", 87, 8, 4, 1, 2, 3 },
};

int main(void)
{
	size_t i;

	pc_begin("XSH.getdate.010");
	gd_setup("%m/%d/%y\n%A %B %d, %Y, %H:%M:%S\n%d,%m,%Y %H:%M\n");
	pc_test();
	for (i = 0; i < GD_N(tab); i++) {
		struct tm r = gd_get(tab[i].in);

		gd_fields(&r, tab[i].in, tab[i].y, tab[i].m, tab[i].d, tab[i].H, tab[i].M, tab[i].S);
	}
	pc_pass();
}
