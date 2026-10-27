/*
 * fc.h - wcstod(), wcstof() and wcstold() behind one signature, and a table
 * driven checker.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef FC_H
#define FC_H

#include "pc.h"
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <wchar.h>

typedef long double (*fc_f)(const wchar_t *, wchar_t **);

static long double w_d(const wchar_t *s, wchar_t **e) { return wcstod(s, e); }
static long double w_f(const wchar_t *s, wchar_t **e) { return wcstof(s, e); }
static long double w_l(const wchar_t *s, wchar_t **e) { return wcstold(s, e); }

struct fc_fn {
	const char *name;
	fc_f f;
	long double huge;	/* HUGE_VAL of the type */
	long double minnorm;	/* smallest normalized positive number */
};

static const struct fc_fn fc_fns[3] = {
	{ "wcstod", w_d, HUGE_VAL, DBL_MIN },
	{ "wcstof", w_f, HUGE_VALF, FLT_MIN },
	{ "wcstold", w_l, HUGE_VALL, LDBL_MIN },
};

/* A conversion expected to succeed: value and number of wide characters consumed (-1: all). */
struct fc_case {
	const wchar_t *in;
	long double val;
	int end;
};

static void fc_run(const struct fc_case *c, int n)
{
	int j, k, err;
	wchar_t *e;
	long double r;

	for (j = 0; j < 3; j++)
		for (k = 0; k < n; k++) {
			e = NULL;
			errno = 0;
			r = fc_fns[j].f(c[k].in, &e);
			err = errno;
			pc_check(r == c[k].val, "%s(\"%ls\") returned %g, expected %g",
				fc_fns[j].name, c[k].in, (double)r, (double)c[k].val);
			pc_check(e == c[k].in + (c[k].end < 0 ? wcslen(c[k].in) : (size_t)c[k].end),
				"%s(\"%ls\") left the end pointer at offset %ld", fc_fns[j].name, c[k].in, (long)(e - c[k].in));
			pc_check(err == 0, "%s(\"%ls\") changed errno to %s", fc_fns[j].name, c[k].in, pc_errname(err));
		}
}

#endif
