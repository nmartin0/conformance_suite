/*
 * j0(+Inf), j1(+Inf), jn(0, +Inf) and jn(3, +Inf) return +0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.j0.003
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
		volatile double r;
		CALLP(r, j0(v0), 0);
		pc_check(IS_PZERO(r), "j0(inf) returned %Lg, expected +0", (long double)r);
	}
	{
		volatile double v0 = (double)INFINITY;
		volatile double r;
		CALLP(r, j1(v0), 0);
		pc_check(IS_PZERO(r), "j1(inf) returned %Lg, expected +0", (long double)r);
	}
	{
		volatile int v0 = 0;
		volatile double v1 = (double)INFINITY;
		volatile double r;
		CALLP(r, jn(v0, v1), 0);
		pc_check(IS_PZERO(r), "jn(0, inf) returned %Lg, expected +0", (long double)r);
	}
	{
		volatile int v0 = 3;
		volatile double v1 = (double)INFINITY;
		volatile double r;
		CALLP(r, jn(v0, v1), 0);
		pc_check(IS_PZERO(r), "jn(3, inf) returned %Lg, expected +0", (long double)r);
	}
}

int main(void)
{
	pc_begin("XSH.j0.003");
#ifdef NO_IEC
	pc_notinuse("IEC 60559 floating-point is not supported");
#endif
	pc_test();
	run_d();
	pc_pass();
}
