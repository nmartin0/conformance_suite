/*
 * hypot returns +Inf if either argument is +Inf or -Inf, even if the
 *   other is a NaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.hypot.003
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
		volatile double v0 = (double)INFINITY;
		volatile double v1 = 2.0;
		volatile double r;
		CALLP(r, hypot(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(inf, 2.0) [double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile double v0 = -(double)INFINITY;
		volatile double v1 = 2.0;
		volatile double r;
		CALLP(r, hypot(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(-inf, 2.0) [double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile double v0 = 2.0;
		volatile double v1 = (double)INFINITY;
		volatile double r;
		CALLP(r, hypot(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(2.0, inf) [double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile double v0 = 2.0;
		volatile double v1 = -(double)INFINITY;
		volatile double r;
		CALLP(r, hypot(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(2.0, -inf) [double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile double v0 = (double)INFINITY;
		volatile double v1 = (double)NAN;
		volatile double r;
		CALLP(r, hypot(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(inf, nan) [double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile double v0 = (double)NAN;
		volatile double v1 = -(double)INFINITY;
		volatile double r;
		CALLP(r, hypot(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(nan, -inf) [double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile double v0 = (double)INFINITY;
		volatile double v1 = -(double)INFINITY;
		volatile double r;
		CALLP(r, hypot(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(inf, -inf) [double] returned %Lg, expected +Inf", (long double)r);
	}
}

static void run_f(void)
{
	{
		volatile float v0 = (float)INFINITY;
		volatile float v1 = 2.0f;
		volatile float r;
		CALLP(r, hypotf(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(inf, 2.0) [float] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile float v0 = -(float)INFINITY;
		volatile float v1 = 2.0f;
		volatile float r;
		CALLP(r, hypotf(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(-inf, 2.0) [float] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile float v0 = 2.0f;
		volatile float v1 = (float)INFINITY;
		volatile float r;
		CALLP(r, hypotf(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(2.0, inf) [float] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile float v0 = 2.0f;
		volatile float v1 = -(float)INFINITY;
		volatile float r;
		CALLP(r, hypotf(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(2.0, -inf) [float] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile float v0 = (float)INFINITY;
		volatile float v1 = (float)NAN;
		volatile float r;
		CALLP(r, hypotf(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(inf, nan) [float] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile float v0 = (float)NAN;
		volatile float v1 = -(float)INFINITY;
		volatile float r;
		CALLP(r, hypotf(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(nan, -inf) [float] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile float v0 = (float)INFINITY;
		volatile float v1 = -(float)INFINITY;
		volatile float r;
		CALLP(r, hypotf(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(inf, -inf) [float] returned %Lg, expected +Inf", (long double)r);
	}
}

static void run_ld(void)
{
	{
		volatile long double v0 = (long double)INFINITY;
		volatile long double v1 = 2.0L;
		volatile long double r;
		CALLP(r, hypotl(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(inf, 2.0) [long double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile long double v0 = -(long double)INFINITY;
		volatile long double v1 = 2.0L;
		volatile long double r;
		CALLP(r, hypotl(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(-inf, 2.0) [long double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile long double v0 = 2.0L;
		volatile long double v1 = (long double)INFINITY;
		volatile long double r;
		CALLP(r, hypotl(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(2.0, inf) [long double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile long double v0 = 2.0L;
		volatile long double v1 = -(long double)INFINITY;
		volatile long double r;
		CALLP(r, hypotl(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(2.0, -inf) [long double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile long double v0 = (long double)INFINITY;
		volatile long double v1 = (long double)NAN;
		volatile long double r;
		CALLP(r, hypotl(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(inf, nan) [long double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile long double v0 = (long double)NAN;
		volatile long double v1 = -(long double)INFINITY;
		volatile long double r;
		CALLP(r, hypotl(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(nan, -inf) [long double] returned %Lg, expected +Inf", (long double)r);
	}
	{
		volatile long double v0 = (long double)INFINITY;
		volatile long double v1 = -(long double)INFINITY;
		volatile long double r;
		CALLP(r, hypotl(v0, v1), 0);
		pc_check(IS_PINF(r), "hypot(inf, -inf) [long double] returned %Lg, expected +Inf", (long double)r);
	}
}

int main(void)
{
	pc_begin("XSH.hypot.003");
#ifdef NO_IEC
	pc_notinuse("IEC 60559 floating-point is not supported");
#endif
	pc_test();
	run_d();
	run_f();
	run_ld();
	pc_pass();
}
