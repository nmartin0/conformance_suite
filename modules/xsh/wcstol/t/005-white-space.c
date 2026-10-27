/*
 * wcstol: white space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstol.005
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const wchar_t ws[6] = { L' ', L'\t', L'\n', L'\v', L'\f', L'\r' };

int main(void)
{
	int k;
	wchar_t in[8];
	struct ic_case c[1];
	pc_begin("XSH.wcstol.005");
	pc_test();
	for (k = 0; k < 6; k++) {
		in[0] = ws[k]; in[1] = L'1'; in[2] = L'2'; in[3] = L'\0';
		c[0].in = in; c[0].base = 10; c[0].val = 12; c[0].end = -1;
		ic_run(c, 1);
	}
	c[0].in = L" \t\n\v\f\r12"; c[0].base = 10; c[0].val = 12; c[0].end = -1;
	ic_run(c, 1);
	c[0].in = L"  +12"; c[0].end = -1;
	ic_run(c, 1);
	c[0].in = L"12 "; c[0].end = 2;
	ic_run(c, 1);
	pc_pass();
}
