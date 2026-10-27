/*
 * strtod: null endptr.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.012
 */
#include "fc.h"
#include <string.h>

int main(void)
{
	int j;
	long double r;
	pc_begin("XSH.strtod.012");
	pc_test();
	for (j = 0; j < 3; j++) {
		r = fc_fns[j].f("3.5xyz", NULL);
		pc_check(r == 3.5L, "%s(\"3.5xyz\", NULL) returned the wrong value", fc_fns[j].name);
		r = fc_fns[j].f("junk", NULL);
		pc_check(r == 0.0L, "%s(\"junk\", NULL) returned non-zero", fc_fns[j].name);
	}
	pc_pass();
}
