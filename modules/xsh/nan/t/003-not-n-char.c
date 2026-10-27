/*
 * nan(tagp) without an n-char sequence matches strtod("NAN").
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nan.003
 */
#include "mt.h"

/* The tag goes through a volatile pointer so the call cannot be folded away. */
static const char *volatile mt_tag;

static void run_d(void)
{
	static const char *const tags[] = { "!", "a b" };
	volatile double r;
	double s;
	int i;

	s = strtod("NAN", NULL);
	pc_check(isnan(s), "strtod(\"NAN\") gave %g, not a NaN", (double)s);
	for (i = 0; i < 2; i++) {
		r = (mt_tag = tags[i], nan(mt_tag));
		pc_check(isnan(r), "nan(\"%s\") returned %g, not a NaN", tags[i], (double)r);
		if (sizeof(double) == sizeof(double) || sizeof(double) == sizeof(float)) {
			double rr = r;
			pc_check(memcmp(&rr, &s, sizeof rr) == 0, "nan(\"%s\") and strtod(\"NAN\") differ in representation", tags[i]);
		}
	}
}

static void run_f(void)
{
	static const char *const tags[] = { "!", "a b" };
	volatile float r;
	float s;
	int i;

	s = strtof("NAN", NULL);
	pc_check(isnan(s), "strtof(\"NAN\") gave %g, not a NaN", (double)s);
	for (i = 0; i < 2; i++) {
		r = (mt_tag = tags[i], nanf(mt_tag));
		pc_check(isnan(r), "nanf(\"%s\") returned %g, not a NaN", tags[i], (double)r);
		if (sizeof(float) == sizeof(double) || sizeof(float) == sizeof(float)) {
			float rr = r;
			pc_check(memcmp(&rr, &s, sizeof rr) == 0, "nanf(\"%s\") and strtof(\"NAN\") differ in representation", tags[i]);
		}
	}
}

static void run_l(void)
{
	static const char *const tags[] = { "!", "a b" };
	volatile long double r;
	long double s;
	int i;

	s = strtold("NAN", NULL);
	pc_check(isnan(s), "strtold(\"NAN\") gave %g, not a NaN", (double)s);
	for (i = 0; i < 2; i++) {
		r = (mt_tag = tags[i], nanl(mt_tag));
		pc_check(isnan(r), "nanl(\"%s\") returned %g, not a NaN", tags[i], (double)r);
		if (sizeof(long double) == sizeof(double) || sizeof(long double) == sizeof(float)) {
			long double rr = r;
			pc_check(memcmp(&rr, &s, sizeof rr) == 0, "nanl(\"%s\") and strtold(\"NAN\") differ in representation", tags[i]);
		}
	}
}

int main(void)
{
	pc_begin("XSH.nan.003");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
