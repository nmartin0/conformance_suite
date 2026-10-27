/*
 * strtod: infinity.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.004
 */
#include "fc.h"
#include <string.h>

static const struct { const char *in; int neg; int end; } cs[] = {
	{ "inf", 0, -1 }, { "INF", 0, -1 }, { "InFiNiTy", 0, -1 }, { "infinity", 0, -1 },
	{ "-inf", 1, -1 }, { "-INFINITY", 1, -1 }, { "+inf", 0, -1 }, { "infin", 0, 3 }
};

int main(void)
{
	int j, k;
	char *e;
	long double r;
	pc_begin("XSH.strtod.004");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < (int)(sizeof cs / sizeof cs[0]); k++) {
			e = NULL;
			r = fc_fns[j].f(cs[k].in, &e);
			pc_check(r == (cs[k].neg ? -fc_fns[j].huge : fc_fns[j].huge), "%s(\"%s\") did not return the infinite value", fc_fns[j].name, cs[k].in);
			pc_check(e == cs[k].in + (cs[k].end < 0 ? strlen(cs[k].in) : (size_t)cs[k].end), "%s(\"%s\") left the end pointer at offset %ld", fc_fns[j].name, cs[k].in, (long)(e - cs[k].in));
		}
	pc_pass();
}
