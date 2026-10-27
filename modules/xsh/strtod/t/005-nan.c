/*
 * strtod: nan.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.005
 */
#include "fc.h"
#include <string.h>

static const struct { const char *in; int end; } cs[] = {
	{ "nan", 3 }, { "NAN", 3 }, { "NaN(abc_1)", 10 }, { "-nan", 4 }, { "nan(0x1)", 8 },
	{ "nan()", 5 }, { "nan(", 3 }, { "nan(!)", 3 }, { "nan(a b)", 3 }
};

int main(void)
{
	int j, k;
	char *e;
	long double r;
	pc_begin("XSH.strtod.005");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < (int)(sizeof cs / sizeof cs[0]); k++) {
			e = NULL;
			r = fc_fns[j].f(cs[k].in, &e);
			if (e == cs[k].in) {
				pc_check(r == 0.0L, "%s(\"%s\") made no conversion but did not return 0", fc_fns[j].name, cs[k].in);
				continue;
			}
			pc_check(r != r, "%s(\"%s\") converted but did not return a NaN", fc_fns[j].name, cs[k].in);
			pc_check(e == cs[k].in + cs[k].end, "%s(\"%s\") left the end pointer at offset %ld", fc_fns[j].name, cs[k].in, (long)(e - cs[k].in));
		}
	pc_pass();
}
