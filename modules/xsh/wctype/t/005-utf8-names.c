/*
 * wctype: utf8 names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctype.005
 */
#include "wc.h"
#include <string.h>

static const char *const nm[12] = { "alnum", "alpha", "blank", "cntrl", "digit", "graph", "lower", "print", "punct", "space", "upper", "xdigit" };
static const int mem[12] = { 97, 97, 32, -1, 48, 97, 97, 97, -1, 32, 65, 48 };

int main(void)
{
	int k;
	wctype_t h;
	pc_begin("XSH.wctype.005");
	wc_init();
	wc_utf8();
	pc_test();
	for (k = 0; k < 12; k++) {
		h = wctype(nm[k]);
		pc_check(h != (wctype_t)0, "wctype(\"%s\") returned 0", nm[k]);
		if (mem[k] >= 0)
			pc_check(iswctype(wc_ch[mem[k]], h) != 0, "iswctype() with the %s handle gave 0 for position %d", nm[k], mem[k]);
	}
	pc_pass();
}
