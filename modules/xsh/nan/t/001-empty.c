/*
 * nan("") returns a quiet NaN like strtod("NAN()").
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nan.001
 */
#include "mt.h"

/* The tag goes through a volatile pointer so the call cannot be folded away. */
static const char *volatile mt_tag;

static void run_d(void)
{
	volatile double r;
	double s;

	r = (mt_tag = "", nan(mt_tag));
#if MT_NO_NAN
	pc_check(r == 0, "nan(\"\") returned %g, expected zero without quiet NaNs", (double)r);
#else
	pc_check(isnan(r), "nan(\"\") returned %g, not a NaN", (double)r);
	s = strtod("NAN()", NULL);
	pc_check(isnan(s), "strtod(\"NAN()\") gave %g, not a NaN", (double)s);
	if (sizeof(double) == sizeof(double) || sizeof(double) == sizeof(float)) {
		double rr = r;
		pc_check(memcmp(&rr, &s, sizeof rr) == 0, "nan(\"\") and strtod(\"NAN()\") differ in representation");
	}
#endif
	(void)s;
}

static void run_f(void)
{
	volatile float r;
	float s;

	r = (mt_tag = "", nanf(mt_tag));
#if MT_NO_NAN
	pc_check(r == 0, "nanf(\"\") returned %g, expected zero without quiet NaNs", (double)r);
#else
	pc_check(isnan(r), "nanf(\"\") returned %g, not a NaN", (double)r);
	s = strtof("NAN()", NULL);
	pc_check(isnan(s), "strtof(\"NAN()\") gave %g, not a NaN", (double)s);
	if (sizeof(float) == sizeof(double) || sizeof(float) == sizeof(float)) {
		float rr = r;
		pc_check(memcmp(&rr, &s, sizeof rr) == 0, "nanf(\"\") and strtof(\"NAN()\") differ in representation");
	}
#endif
	(void)s;
}

static void run_l(void)
{
	volatile long double r;
	long double s;

	r = (mt_tag = "", nanl(mt_tag));
#if MT_NO_NAN
	pc_check(r == 0, "nanl(\"\") returned %g, expected zero without quiet NaNs", (double)r);
#else
	pc_check(isnan(r), "nanl(\"\") returned %g, not a NaN", (double)r);
	s = strtold("NAN()", NULL);
	pc_check(isnan(s), "strtold(\"NAN()\") gave %g, not a NaN", (double)s);
	if (sizeof(long double) == sizeof(double) || sizeof(long double) == sizeof(float)) {
		long double rr = r;
		pc_check(memcmp(&rr, &s, sizeof rr) == 0, "nanl(\"\") and strtold(\"NAN()\") differ in representation");
	}
#endif
	(void)s;
}

int main(void)
{
	pc_begin("XSH.nan.001");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
