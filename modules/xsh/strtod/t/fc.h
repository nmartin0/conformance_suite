/*
 * fc.h - strtod(), strtof() and strtold() behind one signature, and a table
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
#include <string.h>

typedef long double (*fc_f)(const char *, char **);

static double (*volatile v_d)(const char *restrict, char **restrict) = strtod;
static float (*volatile v_f)(const char *restrict, char **restrict) = strtof;
static long double (*volatile v_l)(const char *restrict, char **restrict) = strtold;
static long double w_d(const char *s, char **e) { return v_d(s, e); }
static long double w_f(const char *s, char **e) { return v_f(s, e); }
static long double w_l(const char *s, char **e) { return v_l(s, e); }

struct fc_fn {
	const char *name;
	fc_f f;
	long double huge;	/* HUGE_VAL of the type */
	long double minnorm;	/* smallest normalized positive number */
};

static const struct fc_fn fc_fns[3] = {
	{ "strtod", w_d, HUGE_VAL, DBL_MIN },
	{ "strtof", w_f, HUGE_VALF, FLT_MIN },
	{ "strtold", w_l, HUGE_VALL, LDBL_MIN },
};

/* A conversion expected to succeed: value and number of bytes consumed (-1: all). */
struct fc_case {
	const char *in;
	long double val;
	int end;
};

static void fc_run(const struct fc_case *c, int n)
{
	int j, k, err;
	char *e;
	long double r;

	for (j = 0; j < 3; j++)
		for (k = 0; k < n; k++) {
			e = NULL;
			errno = 0;
			r = fc_fns[j].f(c[k].in, &e);
			err = errno;
			pc_check(r == c[k].val, "%s(\"%s\") returned %g, expected %g",
				fc_fns[j].name, c[k].in, (double)r, (double)c[k].val);
			pc_check(e == c[k].in + (c[k].end < 0 ? strlen(c[k].in) : (size_t)c[k].end),
				"%s(\"%s\") left the end pointer at offset %ld", fc_fns[j].name, c[k].in, (long)(e - c[k].in));
			pc_check(err == 0, "%s(\"%s\") changed errno to %s", fc_fns[j].name, c[k].in, pc_errname(err));
		}
}

#endif
