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
	int k;
	pc_begin("XSH.wctype.001");
	pc_test();
	for (k = 0; k < 12; k++)
		pc_check(wctype(nm[k]) != (wctype_t)0, "wctype(\"%s\") returned 0", nm[k]);
	pc_pass();
}
