/*
 * The 12-hour conversions %I %p and %r, and an hour alone sets minutes and seconds to zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdate.012
 */
#include "gd.h"

static const struct { const char *tmpl, *in; int H, M, S; } tab[] = {
	{ "%I:%M:%S %p", "04:05:06 PM", 16, 5, 6 },
	{ "%I:%M:%S %p", "04:05:06 am", 4, 5, 6 },
	{ "%I:%M:%S %p", "12:00:00 AM", 0, 0, 0 },
	{ "%I:%M:%S %p", "12:30:00 PM", 12, 30, 0 },
	{ "%r", "04:05:06 pm", 16, 5, 6 },
	{ "%m/%d/%y %I %p", "10/1/87 4 PM", 16, 0, 0 },
	{ "%m/%d/%y %I %p", "10/1/87 11 am", 11, 0, 0 },
};

int main(void)
{
	size_t i;
	char line[64];

	pc_begin("XSH.getdate.012");
	gd_setup("%r\n");
	pc_test();
	for (i = 0; i < GD_N(tab); i++) {
		struct tm r;

		snprintf(line, sizeof line, "%s\n", tab[i].tmpl);
		gd_setup(line);
		r = gd_get(tab[i].in);
		gd_fields(&r, tab[i].in, -1, -1, -1, tab[i].H, tab[i].M, tab[i].S);
	}
	pc_pass();
}
