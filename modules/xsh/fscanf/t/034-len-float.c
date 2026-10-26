/*
 * l and L with a floating conversion store a double and a long double.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.034
 * MUTANT    fscanf.lf_float_precision
 * MUTANT    fscanf.ld_double_precision
 * VARIANT   fscanf.float_ulp
 */
#include "pc.h"
#include <errno.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* Relative 4*EPSILON: a practical cap; the text sets no bound (ISO C makes the accuracy implementation-defined). */
static int near_d(double x, double want)
{
	double d = x > want ? x - want : want - x;

	return d <= 4 * DBL_EPSILON * (want < 0 ? -want : want);
}

static int near_ld(long double x, long double want)
{
	long double d = x > want ? x - want : want - x;

	return d <= 4 * LDBL_EPSILON * (want < 0 ? -want : want);
}

int main(void)
{
	double d = 0;
	long double ld = 0;
	int r;

	pc_begin("XSH.fscanf.034");
	pc_test();
	r = sscanf("16777217", "%lf", &d);
	pc_check(r == 1 && near_d(d, 16777217.0), "\"%%lf\" on \"16777217\" returned %d and %.17g", r, d);
	r = sscanf("0.5", "%Lf", &ld);
	pc_check(r == 1 && near_ld(ld, 0.5L), "\"%%Lf\" on \"0.5\" returned %d and %Lg", r, ld);
#if LDBL_MANT_DIG > DBL_MANT_DIG
	/* 2^53 + 1: exact in this long double, not in a double */
	ld = 0;
	r = sscanf("9007199254740993", "%Lf", &ld);
	pc_check(r == 1 && near_ld(ld, 9007199254740993.0L),
	    "\"%%Lf\" on \"9007199254740993\" returned %d and %.21Lg", r, ld);
#endif
	pc_pass();
}
