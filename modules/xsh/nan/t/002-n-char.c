/*
 * nan(tagp) with an n-char sequence matches strtod("NAN(tagp)").
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nan.002
 */
#include "mt.h"

/* The tag goes through a volatile pointer so the call cannot be folded away. */
static const char *volatile mt_tag;

static void run_d(void)
{
	static const char *const tags[] = { "1", "abc", "A_9", "0x1f" };
	static const char *const strs[] = { "NAN(1)", "NAN(abc)", "NAN(A_9)", "NAN(0x1f)" };
	volatile double r;
	double s;
	int i;

	for (i = 0; i < 4; i++) {
		r = (mt_tag = tags[i], nan(mt_tag));
		s = strtod(strs[i], NULL);
		pc_check(isnan(r), "nan(\"%s\") returned %g, not a NaN", tags[i], (double)r);
		pc_check(isnan(s), "strtod(\"%s\") gave %g, not a NaN", strs[i], (double)s);
		if (sizeof(double) == sizeof(double) || sizeof(double) == sizeof(float)) {
			double rr = r;
			pc_check(memcmp(&rr, &s, sizeof rr) == 0, "nan(\"%s\") and strtod(\"%s\") differ in representation", tags[i], strs[i]);
		}
	}
}

static void run_f(void)
{
	static const char *const tags[] = { "1", "abc", "A_9", "0x1f" };
	static const char *const strs[] = { "NAN(1)", "NAN(abc)", "NAN(A_9)", "NAN(0x1f)" };
	volatile float r;
	float s;
	int i;

	for (i = 0; i < 4; i++) {
		r = (mt_tag = tags[i], nanf(mt_tag));
		s = strtof(strs[i], NULL);
		pc_check(isnan(r), "nanf(\"%s\") returned %g, not a NaN", tags[i], (double)r);
		pc_check(isnan(s), "strtof(\"%s\") gave %g, not a NaN", strs[i], (double)s);
		if (sizeof(float) == sizeof(double) || sizeof(float) == sizeof(float)) {
			float rr = r;
			pc_check(memcmp(&rr, &s, sizeof rr) == 0, "nanf(\"%s\") and strtof(\"%s\") differ in representation", tags[i], strs[i]);
		}
	}
}

static void run_l(void)
{
	static const char *const tags[] = { "1", "abc", "A_9", "0x1f" };
	static const char *const strs[] = { "NAN(1)", "NAN(abc)", "NAN(A_9)", "NAN(0x1f)" };
	volatile long double r;
	long double s;
	int i;

	for (i = 0; i < 4; i++) {
		r = (mt_tag = tags[i], nanl(mt_tag));
		s = strtold(strs[i], NULL);
		pc_check(isnan(r), "nanl(\"%s\") returned %g, not a NaN", tags[i], (double)r);
		pc_check(isnan(s), "strtold(\"%s\") gave %g, not a NaN", strs[i], (double)s);
		if (sizeof(long double) == sizeof(double) || sizeof(long double) == sizeof(float)) {
			long double rr = r;
			pc_check(memcmp(&rr, &s, sizeof rr) == 0, "nanl(\"%s\") and strtold(\"%s\") differ in representation", tags[i], strs[i]);
		}
	}
}

int main(void)
{
	pc_begin("XSH.nan.002");
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
