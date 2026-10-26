/*
 * The floating conversions read back snprintf()'s infinity and NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.030
 * MUTANT    fscanf.no_inf_nan
 * VARIANT   fscanf.printf_long_inf
 */
#include "pc.h"
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
#if !defined(INFINITY) || !defined(NAN)
	pc_begin("XSH.fscanf.030");
	pc_notinuse("<math.h> defines no INFINITY or NAN here");
#else
	static const char *fmts[] = { "%f", "%F" };
	char s[3][64];
	float x;
	int r;
	size_t i, k;

	pc_begin("XSH.fscanf.030");
	for (k = 0; k < 2; k++) {
		/* the strings fprintf() makes; only those are to be read back */
		(void)snprintf(s[0], sizeof s[0], fmts[k], (double)INFINITY);
		(void)snprintf(s[1], sizeof s[1], fmts[k], -(double)INFINITY);
		(void)snprintf(s[2], sizeof s[2], fmts[k], (double)NAN);
		for (i = 0; i < 3; i++)
			if (!isalpha((unsigned char)s[i][s[i][0] == '-']))
				pc_notinuse("snprintf(\"%s\") of an infinity or NaN gave \"%s\"", fmts[k], s[i]);
	}
	pc_test();
	for (k = 0; k < 2; k++) {
		(void)snprintf(s[0], sizeof s[0], fmts[k], (double)INFINITY);
		(void)snprintf(s[1], sizeof s[1], fmts[k], -(double)INFINITY);
		(void)snprintf(s[2], sizeof s[2], fmts[k], (double)NAN);
		x = 0;
		r = sscanf(s[0], "%f", &x);
		pc_check(r == 1 && isinf(x) && x > 0, "\"%%f\" on \"%s\" returned %d and %g", s[0], r, (double)x);
		x = 0;
		r = sscanf(s[1], "%f", &x);
		pc_check(r == 1 && isinf(x) && x < 0, "\"%%f\" on \"%s\" returned %d and %g", s[1], r, (double)x);
		x = 0;
		r = sscanf(s[2], "%f", &x);
		pc_check(r == 1 && isnan(x), "\"%%f\" on \"%s\" returned %d and %g", s[2], r, (double)x);
	}
	pc_pass();
#endif
}
