/*
 * frexp() of the largest and smallest normal values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.frexp.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { DBL_MAX, DBL_MIN, -DBL_MAX, -DBL_MIN };
	volatile double x, r;
	double y;
	int i, j, e;

	for (i = 0; i < 4; i++) {
		x = in[i];
		e = 0;
		r = frexp(x, &e);
		y = r;
		pc_check(y >= 0.5 || y <= -0.5, "frexp(%g, &e) returned fraction %g with magnitude below 1/2", (double)x, (double)y);
		pc_check(y < 1.0 && y > -1.0, "frexp(%g, &e) returned fraction %g with magnitude of 1 or more", (double)x, (double)y);
		for (j = 0; j < e; j++)
			y *= 2;
		for (j = 0; j > e; j--)
			y *= 0.5;
		pc_check(y == in[i], "frexp(%g, &e) returned fraction and exponent %d that do not restore the argument", (double)x, e);
	}
}

static void run_f(void)
{
	static const float in[] = { FLT_MAX, FLT_MIN, -FLT_MAX, -FLT_MIN };
	volatile float x, r;
	float y;
	int i, j, e;

	for (i = 0; i < 4; i++) {
		x = in[i];
		e = 0;
		r = frexpf(x, &e);
		y = r;
		pc_check(y >= 0.5f || y <= -0.5f, "frexpf(%g, &e) returned fraction %g with magnitude below 1/2", (double)x, (double)y);
		pc_check(y < 1.0f && y > -1.0f, "frexpf(%g, &e) returned fraction %g with magnitude of 1 or more", (double)x, (double)y);
		for (j = 0; j < e; j++)
			y *= 2;
		for (j = 0; j > e; j--)
			y *= 0.5f;
		pc_check(y == in[i], "frexpf(%g, &e) returned fraction and exponent %d that do not restore the argument", (double)x, e);
	}
}

static void run_l(void)
{
	static const long double in[] = { LDBL_MAX, LDBL_MIN, -LDBL_MAX, -LDBL_MIN };
	volatile long double x, r;
	long double y;
	int i, j, e;

	for (i = 0; i < 4; i++) {
		x = in[i];
		e = 0;
		r = frexpl(x, &e);
		y = r;
		pc_check(y >= 0.5L || y <= -0.5L, "frexpl(%g, &e) returned fraction %g with magnitude below 1/2", (double)x, (double)y);
		pc_check(y < 1.0L && y > -1.0L, "frexpl(%g, &e) returned fraction %g with magnitude of 1 or more", (double)x, (double)y);
		for (j = 0; j < e; j++)
			y *= 2;
		for (j = 0; j > e; j--)
			y *= 0.5L;
		pc_check(y == in[i], "frexpl(%g, &e) returned fraction and exponent %d that do not restore the argument", (double)x, e);
	}
}

int main(void)
{
	pc_begin("XSH.frexp.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
