/*
 * A NaN operand takes precedence over the other conditions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmod.004
 */
#include "mt.h"

static void run_d(void)
{
	volatile double x, y, r;

	{
		x = (double)NAN;
		y = 0.0;
		MT_START_E();
		r = fmod(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmod(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmod");
	}

	{
		x = (double)INFINITY;
		y = (double)NAN;
		MT_START_E();
		r = fmod(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmod(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmod");
	}

	{
		x = (double)NAN;
		y = (double)INFINITY;
		MT_START_E();
		r = fmod(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmod(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmod");
	}

	{
		x = 0.0;
		y = (double)NAN;
		MT_START_E();
		r = fmod(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmod(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmod");
	}
}

static void run_f(void)
{
	volatile float x, y, r;

	{
		x = (float)NAN;
		y = 0.0f;
		MT_START_E();
		r = fmodf(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmodf(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmodf");
	}

	{
		x = (float)INFINITY;
		y = (float)NAN;
		MT_START_E();
		r = fmodf(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmodf(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmodf");
	}

	{
		x = (float)NAN;
		y = (float)INFINITY;
		MT_START_E();
		r = fmodf(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmodf(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmodf");
	}

	{
		x = 0.0f;
		y = (float)NAN;
		MT_START_E();
		r = fmodf(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmodf(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmodf");
	}
}

static void run_l(void)
{
	volatile long double x, y, r;

	{
		x = (long double)NAN;
		y = 0.0L;
		MT_START_E();
		r = fmodl(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmodl(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmodl");
	}

	{
		x = (long double)INFINITY;
		y = (long double)NAN;
		MT_START_E();
		r = fmodl(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmodl(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmodl");
	}

	{
		x = (long double)NAN;
		y = (long double)INFINITY;
		MT_START_E();
		r = fmodl(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmodl(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmodl");
	}

	{
		x = 0.0L;
		y = (long double)NAN;
		MT_START_E();
		r = fmodl(x, y);
		MT_SAVE();
		pc_check(isnan(r), "fmodl(%g, %g) returned %g, not a NaN", (double)x, (double)y, (double)r);
		mt_noerr("fmodl");
	}
}

int main(void)
{
	pc_begin("XSH.fmod.004");
	MT_NEED_MX();
	MT_NEED_NAN();
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
