/*
 * Extra white space in the template file or in the input is ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.017
 */
#include "gd.h"

static const struct { const char *in; int y, m, d; } tab[] = {
	{ "11/27/86", 86, 10, 27 },
	{ "   11/27/86   ", 86, 10, 27 },
	{ "Nov    27,   1986", 86, 10, 27 },
	{ "  Nov 27, 1986", 86, 10, 27 },
};

int main(void)
{
	size_t i;

	pc_begin("XSH.getdate.017");
	gd_setup("  %m/%d/%y   \n%b   %d,  %Y\n");
	pc_test();
	for (i = 0; i < GD_N(tab); i++) {
		struct tm r = gd_get(tab[i].in);

		gd_fields(&r, tab[i].in, tab[i].y, tab[i].m, tab[i].d, -1, -1, -1);
	}
	pc_pass();
}
