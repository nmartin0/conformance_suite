/*
 * atan2(y, +0) and atan2(y, -0) return pi/2 for y = 1 and 2.5 and -pi/2
 *   for y = -1 and -2.5, as the type represents pi/2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atan2.005
 */
#include "pc.h"
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <math.h>

#pragma STDC FENV_ACCESS ON

#if !defined(__STDC_IEC_559__) && !defined(__STDC_IEC_60559_BFP__)
#define NO_IEC 1
#endif
#ifndef INFINITY
#define INFINITY HUGE_VALF
#endif
#ifndef NAN
#define NAN (HUGE_VALF - HUGE_VALF)
#endif
#ifndef FE_INVALID
#define FE_INVALID 0
#endif
#ifndef FE_DIVBYZERO
#define FE_DIVBYZERO 0
#endif
#ifndef FE_OVERFLOW
#define FE_OVERFLOW 0
#endif
#define ERRFLAGS (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW)

#define IS_PZERO(r) ((r) == 0 && !signbit(r))
#define IS_NZERO(r) ((r) == 0 && signbit(r))
#define IS_PINF(r) (isinf(r) && !signbit(r))
#define IS_NINF(r) (isinf(r) && signbit(r))
#define SAME(r, x) ((x) == 0 ? ((r) == 0 && !signbit(r) == !signbit(x)) : (r) == (x))
#ifdef NO_IEC
#define UF_OK(r, mn) ((r) >= -(mn) && (r) <= (mn))
#else
#define UF_OK(r, mn) ((r) == 0)
#endif

static int e_after, f_after;

/* Clears the flags, sets errno to pre, calls, and records errno and flags at once. */
#define CALLP(r, call, pre) do { \
	feclearexcept(FE_ALL_EXCEPT); \
	errno = (pre); \
	(r) = (call); \
	e_after = errno; \
	f_after = fetestexcept(FE_ALL_EXCEPT); \
} while (0)

/* The error is reported by errno and by the flag, each as math_errhandling says. */
#define ERRCHK(what, ecode, flag) do { \
	if (math_errhandling & MATH_ERRNO) \
		pc_check(e_after == (ecode), "%s: errno is %s, expected %s", what, pc_errname(e_after), pc_errname(ecode)); \
	if (math_errhandling & MATH_ERREXCEPT) \
		pc_check((f_after & (flag)) != 0, "%s: exception %s was not raised", what, #flag); \
} while (0)

/* No error: errno is as it was, and no error exception is raised. */
#define NOERR(what, pre) do { \
	pc_check(e_after == (pre), "%s: errno changed from %s to %s", what, pc_errname(pre), pc_errname(e_after)); \
	if (math_errhandling & MATH_ERREXCEPT) \
		pc_check((f_after & ERRFLAGS) == 0, "%s: an error exception was raised", what); \
} while (0)

/* A particular error did not occur. */
#define NOTERR(what, ecode, flag) do { \
	pc_check(e_after != (ecode), "%s: errno was set to %s", what, pc_errname(ecode)); \
	if (math_errhandling & MATH_ERREXCEPT) \
		pc_check((f_after & (flag)) == 0, "%s: exception %s was raised", what, #flag); \
} while (0)

#define NEEDMECH() do { \
	if (!(math_errhandling & (MATH_ERRNO | MATH_ERREXCEPT))) \
		pc_notinuse("math_errhandling reports errors by neither errno nor exceptions"); \
} while (0)

static void run_d(void)
{
	{
		volatile double v0 = 1.0;
		volatile double v1 = 0.0;
		volatile double r;
		CALLP(r, atan2(v0, v1), 0);
		pc_check((r) == (double)(1.57079632679489661923132169163975144), "atan2(1.0, 0.0) [double] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile double v0 = 1.0;
		volatile double v1 = -0.0;
		volatile double r;
		CALLP(r, atan2(v0, v1), 0);
		pc_check((r) == (double)(1.57079632679489661923132169163975144), "atan2(1.0, -0.0) [double] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile double v0 = 2.5;
		volatile double v1 = 0.0;
		volatile double r;
		CALLP(r, atan2(v0, v1), 0);
		pc_check((r) == (double)(1.57079632679489661923132169163975144), "atan2(2.5, 0.0) [double] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile double v0 = 2.5;
		volatile double v1 = -0.0;
		volatile double r;
		CALLP(r, atan2(v0, v1), 0);
		pc_check((r) == (double)(1.57079632679489661923132169163975144), "atan2(2.5, -0.0) [double] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile double v0 = -1.0;
		volatile double v1 = 0.0;
		volatile double r;
		CALLP(r, atan2(v0, v1), 0);
		pc_check((r) == (double)(-1.57079632679489661923132169163975144), "atan2(-1.0, 0.0) [double] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile double v0 = -1.0;
		volatile double v1 = -0.0;
		volatile double r;
		CALLP(r, atan2(v0, v1), 0);
		pc_check((r) == (double)(-1.57079632679489661923132169163975144), "atan2(-1.0, -0.0) [double] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile double v0 = -2.5;
		volatile double v1 = 0.0;
		volatile double r;
		CALLP(r, atan2(v0, v1), 0);
		pc_check((r) == (double)(-1.57079632679489661923132169163975144), "atan2(-2.5, 0.0) [double] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile double v0 = -2.5;
		volatile double v1 = -0.0;
		volatile double r;
		CALLP(r, atan2(v0, v1), 0);
		pc_check((r) == (double)(-1.57079632679489661923132169163975144), "atan2(-2.5, -0.0) [double] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
}

static void run_f(void)
{
	{
		volatile float v0 = 1.0f;
		volatile float v1 = 0.0f;
		volatile float r;
		CALLP(r, atan2f(v0, v1), 0);
		pc_check((r) == (float)(1.57079632679489661923132169163975144f), "atan2(1.0, 0.0) [float] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile float v0 = 1.0f;
		volatile float v1 = -0.0f;
		volatile float r;
		CALLP(r, atan2f(v0, v1), 0);
		pc_check((r) == (float)(1.57079632679489661923132169163975144f), "atan2(1.0, -0.0) [float] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile float v0 = 2.5f;
		volatile float v1 = 0.0f;
		volatile float r;
		CALLP(r, atan2f(v0, v1), 0);
		pc_check((r) == (float)(1.57079632679489661923132169163975144f), "atan2(2.5, 0.0) [float] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile float v0 = 2.5f;
		volatile float v1 = -0.0f;
		volatile float r;
		CALLP(r, atan2f(v0, v1), 0);
		pc_check((r) == (float)(1.57079632679489661923132169163975144f), "atan2(2.5, -0.0) [float] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile float v0 = -1.0f;
		volatile float v1 = 0.0f;
		volatile float r;
		CALLP(r, atan2f(v0, v1), 0);
		pc_check((r) == (float)(-1.57079632679489661923132169163975144f), "atan2(-1.0, 0.0) [float] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile float v0 = -1.0f;
		volatile float v1 = -0.0f;
		volatile float r;
		CALLP(r, atan2f(v0, v1), 0);
		pc_check((r) == (float)(-1.57079632679489661923132169163975144f), "atan2(-1.0, -0.0) [float] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile float v0 = -2.5f;
		volatile float v1 = 0.0f;
		volatile float r;
		CALLP(r, atan2f(v0, v1), 0);
		pc_check((r) == (float)(-1.57079632679489661923132169163975144f), "atan2(-2.5, 0.0) [float] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile float v0 = -2.5f;
		volatile float v1 = -0.0f;
		volatile float r;
		CALLP(r, atan2f(v0, v1), 0);
		pc_check((r) == (float)(-1.57079632679489661923132169163975144f), "atan2(-2.5, -0.0) [float] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
}

static void run_ld(void)
{
	{
		volatile long double v0 = 1.0L;
		volatile long double v1 = 0.0L;
		volatile long double r;
		CALLP(r, atan2l(v0, v1), 0);
		pc_check((r) == (long double)(1.57079632679489661923132169163975144L), "atan2(1.0, 0.0) [long double] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile long double v0 = 1.0L;
		volatile long double v1 = -0.0L;
		volatile long double r;
		CALLP(r, atan2l(v0, v1), 0);
		pc_check((r) == (long double)(1.57079632679489661923132169163975144L), "atan2(1.0, -0.0) [long double] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile long double v0 = 2.5L;
		volatile long double v1 = 0.0L;
		volatile long double r;
		CALLP(r, atan2l(v0, v1), 0);
		pc_check((r) == (long double)(1.57079632679489661923132169163975144L), "atan2(2.5, 0.0) [long double] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile long double v0 = 2.5L;
		volatile long double v1 = -0.0L;
		volatile long double r;
		CALLP(r, atan2l(v0, v1), 0);
		pc_check((r) == (long double)(1.57079632679489661923132169163975144L), "atan2(2.5, -0.0) [long double] returned %Lg, expected 1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile long double v0 = -1.0L;
		volatile long double v1 = 0.0L;
		volatile long double r;
		CALLP(r, atan2l(v0, v1), 0);
		pc_check((r) == (long double)(-1.57079632679489661923132169163975144L), "atan2(-1.0, 0.0) [long double] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile long double v0 = -1.0L;
		volatile long double v1 = -0.0L;
		volatile long double r;
		CALLP(r, atan2l(v0, v1), 0);
		pc_check((r) == (long double)(-1.57079632679489661923132169163975144L), "atan2(-1.0, -0.0) [long double] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile long double v0 = -2.5L;
		volatile long double v1 = 0.0L;
		volatile long double r;
		CALLP(r, atan2l(v0, v1), 0);
		pc_check((r) == (long double)(-1.57079632679489661923132169163975144L), "atan2(-2.5, 0.0) [long double] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
	{
		volatile long double v0 = -2.5L;
		volatile long double v1 = -0.0L;
		volatile long double r;
		CALLP(r, atan2l(v0, v1), 0);
		pc_check((r) == (long double)(-1.57079632679489661923132169163975144L), "atan2(-2.5, -0.0) [long double] returned %Lg, expected -1.57079632679489661923132169163975144", (long double)r);
	}
}

int main(void)
{
	pc_begin("XSH.atan2.005");
	pc_test();
	run_d();
	run_f();
	run_ld();
	pc_pass();
}
