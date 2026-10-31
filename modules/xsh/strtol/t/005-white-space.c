/*
 * strtol: white space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtol.005
 * MUTANT    strtol.ws_none
 * MUTANT    strtol.ws_only_space
 * MUTANT    strtol.ws_no_vt_ff
 * MUTANT    strtol.ws_no_cr
 * MUTANT    strtol.ws_one_only
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const char ws[6] = { ' ', '\t', '\n', '\v', '\f', '\r' };

int main(void)
{
	int k;
	char in[8];
	struct ic_case c[1];
	pc_begin("XSH.strtol.005");
	pc_test();
	for (k = 0; k < 6; k++) {
		in[0] = ws[k]; in[1] = '1'; in[2] = '2'; in[3] = '\0';
		c[0].in = in; c[0].base = 10; c[0].val = 12; c[0].end = -1;
		ic_run(c, 1);
	}
	c[0].in = " \t\n\v\f\r12"; c[0].base = 10; c[0].val = 12; c[0].end = -1;
	ic_run(c, 1);
	c[0].in = "  +12"; c[0].end = -1;
	ic_run(c, 1);
	c[0].in = "12 "; c[0].end = 2;
	ic_run(c, 1);
	pc_pass();
}
