/*
 * wcstol: limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstol.009
 */
#define IC_L
#include "ic.h"
#include <string.h>

/* Adds one to the magnitude of the decimal number in s (a leading minus is kept). */
static void inc_dec(wchar_t *s)
{
	size_t n = wcslen(s);

	while (n > 0) {
		n--;
		if (s[n] == L'9') {
			s[n] = L'0';
			continue;
		}
		if (s[n] >= L'0' && s[n] <= L'8') {
			s[n]++;
			return;
		}
	}
	pc_unresolved("cannot increment a number string");
}

int main(void)
{
	int j, err;
	wchar_t *e;
	R r;
	wchar_t smax[IC_N][64], sover[IC_N][64], smin[IC_N][64], sunder[IC_N][64];
	pc_begin("XSH.wcstol.009");
	for (j = 0; j < IC_N; j++) {
		pc_check(snprintf(smax[j], 64, L"%jd", (intmax_t)ic_fns[j].max) > 0, "swprintf failed");
		strcpy(sover[j], smax[j]);
		inc_dec(sover[j]);
		pc_check(snprintf(smin[j], 64, L"%jd", (intmax_t)ic_fns[j].min) > 0, "swprintf failed");
		strcpy(sunder[j], smin[j]);
		inc_dec(sunder[j]);
	}
	pc_test();
	for (j = 0; j < IC_N; j++) {
		errno = EDOM;
		r = ic_fns[j].f(smax[j], &e, 10);
		err = errno;
		pc_check(r == ic_fns[j].max && err == EDOM && e == smax[j] + wcslen(smax[j]), "%s() of the maximum's digits was not converted exactly", ic_fns[j].name);
		errno = 0;
		r = ic_fns[j].f(sover[j], &e, 10);
		err = errno;
		pc_check(r == ic_fns[j].max && err == ERANGE, "%s() of one above the maximum did not give the maximum with ERANGE", ic_fns[j].name);
		errno = EDOM;
		r = ic_fns[j].f(smin[j], &e, 10);
		err = errno;
		pc_check(r == ic_fns[j].min && err == EDOM && e == smin[j] + wcslen(smin[j]), "%s() of the minimum's digits was not converted exactly", ic_fns[j].name);
		errno = 0;
		r = ic_fns[j].f(sunder[j], &e, 10);
		err = errno;
		pc_check(r == ic_fns[j].min && err == ERANGE, "%s() of one below the minimum did not give the minimum with ERANGE", ic_fns[j].name);
	}
	pc_pass();
}
