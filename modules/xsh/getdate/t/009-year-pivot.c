/*
 * A two-digit year in 69 to 99 is 1969 to 1999 and in 00 to 68 is 2000 to 2068.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.009
 */
#include "gd.h"

int main(void)
{
	static const struct { const char *in; int y; } tab[] = {
		{ "01/02/68", 168 }, { "01/02/69", 69 }, { "01/02/99", 99 }, { "01/02/00", 100 },
		{ "01/02/30", 130 }, { "01/02/70", 70 },
	};
	size_t i;

	pc_begin("XSH.getdate.009");
	gd_setup("%m/%d/%y\n");
	pc_test();
	for (i = 0; i < GD_N(tab); i++) {
		struct tm r;

		/* 2068 is beyond a 32-bit time_t; the page then calls the input invalid. */
		if (tab[i].y == 168 && sizeof(time_t) < 8)
			continue;
		r = gd_get(tab[i].in);
		gd_fields(&r, tab[i].in, tab[i].y, 0, 2, -1, -1, -1);
	}
	pc_pass();
}
