/*
 * In the POSIX locale %x %X and %c have the forms %m/%d/%y, %H:%M:%S and %a %b %e %H:%M:%S %Y.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.018
 */
#include "gd.h"

static const struct { const char *tmpl, *in; int y, m, d, H, M, S; } tab[] = {
	{ "%x", "11/27/86", 86, 10, 27, -1, -1, -1 },
	{ "%x %X", "11/27/86 10:11:12", 86, 10, 27, 10, 11, 12 },
	{ "%c", "Thu Nov 27 10:11:12 1986", 86, 10, 27, 10, 11, 12 },
	{ "%c", "thu nov  5 01:02:03 1987", 87, 10, 5, 1, 2, 3 },
	{ "%X", "10:11:12", -1, -1, -1, 10, 11, 12 },
};

int main(void)
{
	size_t i;
	char line[64];

	pc_begin("XSH.getdate.018");
	gd_setup("%x\n");
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
