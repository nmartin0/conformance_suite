/*
 * wcstoimax: limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstoimax.009
 */
#define IC_IMAX
#include "ic.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* The decimal text of a value, widened digit by digit (no dependence on swprintf()). */
static void wnum(wchar_t *w, const char *fmt, ...)
{
	char t[64];
	size_t i;
	va_list ap;

	va_start(ap, fmt);
	(void)vsnprintf(t, sizeof t, fmt, ap);
	va_end(ap);
	for (i = 0; t[i] != '\0'; i++) {
		wint_t c = btowc((unsigned char)t[i]);

		if (c == WEOF)
			pc_unresolved("btowc failed for a digit");
		w[i] = (wchar_t)c;
	}
	w[i] = L'\0';
}

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
	pc_begin("XSH.wcstoimax.009");
	for (j = 0; j < IC_N; j++) {
		wnum(smax[j], "%jd", (intmax_t)ic_fns[j].max);
		wcscpy(sover[j], smax[j]);
		inc_dec(sover[j]);
		wnum(smin[j], "%jd", (intmax_t)ic_fns[j].min);
		wcscpy(sunder[j], smin[j]);
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
