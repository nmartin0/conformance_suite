/*
 * wcstod: white space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstod.006
 */
#include "fc.h"
#include <string.h>

static const wchar_t ws[6] = { L' ', L'\t', L'\n', L'\v', L'\f', L'\r' };

int main(void)
{
	int k;
	wchar_t in[8];
	struct fc_case c[1];
	pc_begin("XSH.wcstod.006");
	pc_test();
	for (k = 0; k < 6; k++) {
		in[0] = ws[k];
		in[1] = L'4'; in[2] = L'.'; in[3] = L'5'; in[4] = L'\0';
		c[0].in = in; c[0].val = 4.5L; c[0].end = -1;
		fc_run(c, 1);
	}
	c[0].in = L" \t\n\v\f\r4.5"; c[0].val = 4.5L; c[0].end = -1;
	fc_run(c, 1);
	pc_pass();
}
