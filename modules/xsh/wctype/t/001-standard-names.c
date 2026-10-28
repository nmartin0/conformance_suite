/*
 * wctype: standard names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctype.001
 */
#include "wc.h"
#include <string.h>

static const char *const nm[12] = { "alnum", "alpha", "blank", "cntrl", "digit", "graph", "lower", "print", "punct", "space", "upper", "xdigit" };

int main(void)
{
	int k, i;
	wctype_t h;
	pc_begin("XSH.wctype.001");
	wc_init();
	pc_test();
	for (k = 0; k < 12; k++) {
		h = wctype(nm[k]);
		pc_check(h != (wctype_t)0, "wctype(\"%s\") returned 0", nm[k]);
		/* the handle is usable: it separates a member of the class from a character outside it */
		for (i = 0; i < 128 && !wc_in(k, i); i++)
			;
		pc_check(i < 128 && iswctype(wc_ch[i], h) != 0, "the %s handle gave 0 for a member", nm[k]);
		for (i = 0; i < 128 && wc_in(k, i); i++)
			;
		pc_check(i < 128 && iswctype(wc_ch[i], h) == 0, "the %s handle gave non-zero for a non-member", nm[k]);
	}
	pc_pass();
}
