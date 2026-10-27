/*
 * wctype: repeatable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctype.004
 */
#include "wc.h"
#include <string.h>

static const char *const nm[12] = { "alnum", "alpha", "blank", "cntrl", "digit", "graph", "lower", "print", "punct", "space", "upper", "xdigit" };

int main(void)
{
	int k, i;
	wctype_t h1, h2;
	pc_begin("XSH.wctype.004");
	wc_init();
	pc_test();
	for (k = 0; k < 12; k++) {
		h1 = wctype(nm[k]);
		(void)wctype("alpha");
		h2 = wctype(nm[k]);
		pc_check(h1 != (wctype_t)0 && h2 != (wctype_t)0, "wctype(\"%s\") returned 0", nm[k]);
		for (i = 0; i < 128; i++)
			pc_check((iswctype(wc_ch[i], h1) != 0) == (iswctype(wc_ch[i], h2) != 0), "two handles for \"%s\" disagree at position %d", nm[k], i);
	}
	pc_pass();
}
