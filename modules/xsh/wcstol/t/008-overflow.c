/*
 * wcstol: overflow.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstol.008
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const wchar_t pos[] = L"99999999999999999999999999999999999999999";
static const wchar_t neg[] = L"-99999999999999999999999999999999999999999";

int main(void)
{
	int j, err;
	wchar_t *e;
	R r;
	pc_begin("XSH.wcstol.008");
	pc_test();
	for (j = 0; j < IC_N; j++) {
		e = NULL;
		errno = 0;
		r = ic_fns[j].f(pos, &e, 10);
		err = errno;
		pc_check(r == ic_fns[j].max, "%s() of a huge number did not return the maximum", ic_fns[j].name);
		pc_check(err == ERANGE, "%s() of a huge number set errno to %s", ic_fns[j].name, pc_errname(err));
		pc_check(e == pos + wcslen(pos), "%s() left the end pointer at offset %ld", ic_fns[j].name, (long)(e - pos));
		e = NULL;
		errno = 0;
		r = ic_fns[j].f(neg, &e, 10);
		err = errno;
		pc_check(r == ic_fns[j].min, "%s() of a huge negative number returned the wrong value", ic_fns[j].name);
		pc_check(err == ERANGE, "%s() of a huge negative number set errno to %s", ic_fns[j].name, pc_errname(err));
		pc_check(e == neg + wcslen(neg), "%s() left the end pointer at offset %ld (negative)", ic_fns[j].name, (long)(e - neg));
	}
	pc_pass();
}
