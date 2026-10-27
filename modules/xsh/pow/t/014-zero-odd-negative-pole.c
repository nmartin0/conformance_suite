/*
 * pow(+0, y) returns +HUGE_VAL and pow(-0, y) returns -HUGE_VAL for the
 *   odd negative integers y = -1 and -3, as a pole error: errno is
 *   ERANGE where math_errhandling includes MATH_ERRNO, and the divide-
 *   by-zero exception is raised where it includes MATH_ERREXCEPT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pow.014
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
		volatile double v0 = 0.0;
		volatile double v1 = -1.0;
		volatile double r;
		CALLP(r, pow(v0, v1), 0);
		pc_check((r) == HUGE_VAL, "pow(0.0, -1.0) [double] returned %Lg, expected +HUGE_VAL", (long double)r);
		ERRCHK("pow(0.0, -1.0) [double]", ERANGE, FE_DIVBYZERO);
	}
	{
		volatile double v0 = 0.0;
		volatile double v1 = -3.0;
		volatile double r;
		CALLP(r, pow(v0, v1), 0);
		pc_check((r) == HUGE_VAL, "pow(0.0, -3.0) [double] returned %Lg, expected +HUGE_VAL", (long double)r);
		ERRCHK("pow(0.0, -3.0) [double]", ERANGE, FE_DIVBYZERO);
	}
	{
		volatile double v0 = -0.0;
		volatile double v1 = -1.0;
		volatile double r;
		CALLP(r, pow(v0, v1), 0);
		pc_check((r) == -HUGE_VAL, "pow(-0.0, -1.0) [double] returned %Lg, expected -HUGE_VAL", (long double)r);
		ERRCHK("pow(-0.0, -1.0) [double]", ERANGE, FE_DIVBYZERO);
	}
	{
		volatile double v0 = -0.0;
		volatile double v1 = -3.0;
		volatile double r;
		CALLP(r, pow(v0, v1), 0);
		pc_check((r) == -HUGE_VAL, "pow(-0.0, -3.0) [double] returned %Lg, expected -HUGE_VAL", (long double)r);
		ERRCHK("pow(-0.0, -3.0) [double]", ERANGE, FE_DIVBYZERO);
	}
}

static void run_f(void)
{
	{
		volatile float v0 = 0.0f;
		volatile float v1 = -1.0f;
		volatile float r;
		CALLP(r, powf(v0, v1), 0);
		pc_check((r) == HUGE_VALF, "pow(0.0, -1.0) [float] returned %Lg, expected +HUGE_VAL", (long double)r);
		ERRCHK("pow(0.0, -1.0) [float]", ERANGE, FE_DIVBYZERO);
	}
	{
		volatile float v0 = 0.0f;
		volatile float v1 = -3.0f;
		volatile float r;
		CALLP(r, powf(v0, v1), 0);
		pc_check((r) == HUGE_VALF, "pow(0.0, -3.0) [float] returned %Lg, expected +HUGE_VAL", (long double)r);
		ERRCHK("pow(0.0, -3.0) [float]", ERANGE, FE_DIVBYZERO);
	}
	{
		volatile float v0 = -0.0f;
		volatile float v1 = -1.0f;
		volatile float r;
		CALLP(r, powf(v0, v1), 0);
		pc_check((r) == -HUGE_VALF, "pow(-0.0, -1.0) [float] returned %Lg, expected -HUGE_VAL", (long double)r);
		ERRCHK("pow(-0.0, -1.0) [float]", ERANGE, FE_DIVBYZERO);
	}
	{
		volatile float v0 = -0.0f;
		volatile float v1 = -3.0f;
		volatile float r;
		CALLP(r, powf(v0, v1), 0);
		pc_check((r) == -HUGE_VALF, "pow(-0.0, -3.0) [float] returned %Lg, expected -HUGE_VAL", (long double)r);
		ERRCHK("pow(-0.0, -3.0) [float]", ERANGE, FE_DIVBYZERO);
	}
}

static void run_ld(void)
{
	{
		volatile long double v0 = 0.0L;
		volatile long double v1 = -1.0L;
		volatile long double r;
		CALLP(r, powl(v0, v1), 0);
		pc_check((r) == HUGE_VALL, "pow(0.0, -1.0) [long double] returned %Lg, expected +HUGE_VAL", (long double)r);
		ERRCHK("pow(0.0, -1.0) [long double]", ERANGE, FE_DIVBYZERO);
	}
	{
		volatile long double v0 = 0.0L;
		volatile long double v1 = -3.0L;
		volatile long double r;
		CALLP(r, powl(v0, v1), 0);
		pc_check((r) == HUGE_VALL, "pow(0.0, -3.0) [long double] returned %Lg, expected +HUGE_VAL", (long double)r);
		ERRCHK("pow(0.0, -3.0) [long double]", ERANGE, FE_DIVBYZERO);
	}
	{
		volatile long double v0 = -0.0L;
		volatile long double v1 = -1.0L;
		volatile long double r;
		CALLP(r, powl(v0, v1), 0);
		pc_check((r) == -HUGE_VALL, "pow(-0.0, -1.0) [long double] returned %Lg, expected -HUGE_VAL", (long double)r);
		ERRCHK("pow(-0.0, -1.0) [long double]", ERANGE, FE_DIVBYZERO);
	}
	{
		volatile long double v0 = -0.0L;
		volatile long double v1 = -3.0L;
		volatile long double r;
		CALLP(r, powl(v0, v1), 0);
		pc_check((r) == -HUGE_VALL, "pow(-0.0, -3.0) [long double] returned %Lg, expected -HUGE_VAL", (long double)r);
		ERRCHK("pow(-0.0, -3.0) [long double]", ERANGE, FE_DIVBYZERO);
	}
}

int main(void)
{
	pc_begin("XSH.pow.014");
#ifdef NO_IEC
	pc_notinuse("IEC 60559 floating-point is not supported");
#endif
	pc_test();
	run_d();
	run_f();
	run_ld();
	NEEDMECH();
	pc_pass();
}
