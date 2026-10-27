/*
 * wcstod: nan.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstod.005
 */
#include "fc.h"
#include <string.h>

static const struct { const wchar_t *in; int end; } cs[] = {
	{ L"nan", 3 }, { L"NAN", 3 }, { L"NaN(abc_1)", 10 }, { L"-nan", 4 }, { L"nan(0x1)", 8 },
	{ L"nan()", 5 }, { L"nan(", 3 }, { L"nan(!)", 3 }, { L"nan(a b)", 3 }
};

int main(void)
{
	int j, k;
	wchar_t *e;
	long double r;
	pc_begin("XSH.wcstod.005");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < (int)(sizeof cs / sizeof cs[0]); k++) {
			e = NULL;
			r = fc_fns[j].f(cs[k].in, &e);
			if (e == cs[k].in) {
				pc_check(r == 0.0L, "%s(\"%ls\") made no conversion but did not return 0", fc_fns[j].name, cs[k].in);
				continue;
			}
			pc_check(r != r, "%s(\"%ls\") converted but did not return a NaN", fc_fns[j].name, cs[k].in);
			pc_check(e == cs[k].in + cs[k].end, "%s(\"%ls\") left the end pointer at offset %ld", fc_fns[j].name, cs[k].in, (long)(e - cs[k].in));
		}
	pc_pass();
}
