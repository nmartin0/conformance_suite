/*
 * wcstoimax: null endptr u.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstoimax.024
 */
#define IC_UMAX
#include "ic.h"
#include <string.h>

int main(void)
{
	int j;
	pc_begin("XSH.wcstoimax.024");
	pc_test();
	for (j = 0; j < IC_N; j++) {
		pc_check(ic_fns[j].f(L"77xyz", NULL, 10) == 77, "%s(\"77xyz\", NULL) returned the wrong value", ic_fns[j].name);
		pc_check(ic_fns[j].f(L"junk", NULL, 10) == 0, "%s(\"junk\", NULL) returned non-zero", ic_fns[j].name);
	}
	pc_pass();
}
