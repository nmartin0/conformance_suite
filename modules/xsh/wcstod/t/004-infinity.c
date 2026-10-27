/*
 * wcstod: infinity.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstod.004
 */
#include "fc.h"
#include <string.h>

static const struct { const wchar_t *in; int neg; int end; } cs[] = {
	{ L"inf", 0, -1 }, { L"INF", 0, -1 }, { L"InFiNiTy", 0, -1 }, { L"infinity", 0, -1 },
	{ L"-inf", 1, -1 }, { L"-INFINITY", 1, -1 }, { L"+inf", 0, -1 }, { L"infin", 0, 3 }
};

int main(void)
{
	int j, k;
	wchar_t *e;
	long double r;
	pc_begin("XSH.wcstod.004");
	pc_test();
	for (j = 0; j < 3; j++)
		for (k = 0; k < (int)(sizeof cs / sizeof cs[0]); k++) {
			e = NULL;
			r = fc_fns[j].f(cs[k].in, &e);
			pc_check(r == (cs[k].neg ? -fc_fns[j].huge : fc_fns[j].huge), "%s(\"%ls\") did not return the infinite value", fc_fns[j].name, cs[k].in);
			pc_check(e == cs[k].in + (cs[k].end < 0 ? wcslen(cs[k].in) : (size_t)cs[k].end), "%s(\"%ls\") left the end pointer at offset %ld", fc_fns[j].name, cs[k].in, (long)(e - cs[k].in));
		}
	pc_pass();
}
