/*
 * strtod: white space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.006
 */
#include "fc.h"
#include <string.h>

static const char ws[6] = { ' ', '\t', '\n', '\v', '\f', '\r' };

int main(void)
{
	int k;
	char in[8];
	struct fc_case c[1];
	pc_begin("XSH.strtod.006");
	pc_test();
	for (k = 0; k < 6; k++) {
		in[0] = ws[k];
		in[1] = '4'; in[2] = '.'; in[3] = '5'; in[4] = '\0';
		c[0].in = in; c[0].val = 4.5L; c[0].end = -1;
		fc_run(c, 1);
	}
	c[0].in = " \t\n\v\f\r4.5"; c[0].val = 4.5L; c[0].end = -1;
	fc_run(c, 1);
	pc_pass();
}
