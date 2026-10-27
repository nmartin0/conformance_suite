/*
 * wctype: unknown names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctype.002
 */
#include "wc.h"
#include <string.h>

static const char *const bad[4] = { "", "no such class", "alpha ", "al pha" };

int main(void)
{
	int k;
	pc_begin("XSH.wctype.002");
	pc_test();
	for (k = 0; k < 4; k++)
		pc_check(wctype(bad[k]) == (wctype_t)0, "wctype(\"%s\") returned non-zero", bad[k]);
	pc_pass();
}
