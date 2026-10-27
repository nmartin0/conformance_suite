/*
 * mt.h - helpers shared by the tests of one mathematics module.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Arguments and results of the calls under test go through volatile
 * objects so that nothing is folded at translation time. Errors are
 * judged as XBD 4.23 describes: errno only when (math_errhandling &
 * MATH_ERRNO) is non-zero, exception flags only when
 * (math_errhandling & MATH_ERREXCEPT) is non-zero.
 */
#ifndef MT_H
#define MT_H

#include "pc.h"
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT OFF

/* The IEC 60559 Floating-Point option, as the C implementation claims it. */
#if defined(__STDC_IEC_559__) || defined(__STDC_IEC_60559_BFP__)
#define MT_MX 1
#else
#define MT_MX 0
#endif

#ifndef NAN
#define NAN 0.0f
#define MT_NO_NAN 1
#else
#define MT_NO_NAN 0
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
#ifndef FE_UNDERFLOW
#define FE_UNDERFLOW 0
#endif
#ifdef FE_INEXACT
#define MT_HAVE_INEXACT 1
#else
#define MT_HAVE_INEXACT 0
#define FE_INEXACT 0
#endif

/* Rounding directions; the sentinels (negative) match no real mode. */
#ifdef FE_DOWNWARD
#define MT_DOWN FE_DOWNWARD
#else
#define MT_DOWN (-2)
#endif
#ifdef FE_UPWARD
#define MT_UP FE_UPWARD
#else
#define MT_UP (-3)
#endif
#ifdef FE_TOWARDZERO
#define MT_ZERO FE_TOWARDZERO
#else
#define MT_ZERO (-4)
#endif
#ifdef FE_TONEAREST
#define MT_NEAR FE_TONEAREST
#else
#define MT_NEAR (-5)
#endif

static const int mt_modes[] = {
#ifdef FE_TONEAREST
	FE_TONEAREST,
#endif
#ifdef FE_DOWNWARD
	FE_DOWNWARD,
#endif
#ifdef FE_UPWARD
	FE_UPWARD,
#endif
#ifdef FE_TOWARDZERO
	FE_TOWARDZERO,
#endif
	-1
};
#define MT_NMODES ((int)(sizeof mt_modes / sizeof mt_modes[0]) - 1)

static int mt_e, mt_fl;		/* errno and flags saved right after a call */
static int mt_orig_mode = -1, mt_tried;

static int mt_errno_on(void) { return (math_errhandling & MATH_ERRNO) != 0; }
static int mt_exc_on(void) { return (math_errhandling & MATH_ERREXCEPT) != 0; }

/* Immediately before the call: errno 0 and no flag raised. */
#define MT_START() do { errno = 0; (void)feclearexcept(FE_ALL_EXCEPT); } while (0)
/* The same, with errno holding a non-zero value that must survive. */
#define MT_START_E() do { errno = EDOM; (void)feclearexcept(FE_ALL_EXCEPT); } while (0)
/* Immediately after the call. */
#define MT_SAVE() do { mt_e = errno; mt_fl = fetestexcept(FE_ALL_EXCEPT); } while (0)

#define MT_ERRFLAGS (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW)

/* Checks of the way an error is reported (XBD 4.23.1 to 4.23.3). */
static void mt_domain(const char *what)
{
	if (mt_errno_on())
		pc_check(mt_e == EDOM, "%s: domain error, errno is %s, expected EDOM", what, pc_errname(mt_e));
	if (mt_exc_on())
		pc_check((mt_fl & FE_INVALID) != 0, "%s: domain error, invalid exception not raised", what);
}

static void mt_pole(const char *what)
{
	if (mt_errno_on())
		pc_check(mt_e == ERANGE, "%s: pole error, errno is %s, expected ERANGE", what, pc_errname(mt_e));
	if (mt_exc_on())
		pc_check((mt_fl & FE_DIVBYZERO) != 0, "%s: pole error, divide-by-zero exception not raised", what);
}

static void mt_overflow(const char *what)
{
	if (mt_errno_on())
		pc_check(mt_e == ERANGE, "%s: overflow, errno is %s, expected ERANGE", what, pc_errname(mt_e));
	if (mt_exc_on())
		pc_check((mt_fl & FE_OVERFLOW) != 0, "%s: overflow, overflow exception not raised", what);
}

static void mt_underflow(const char *what)
{
	if (mt_errno_on())
		pc_check(mt_e == ERANGE, "%s: underflow, errno is %s, expected ERANGE", what, pc_errname(mt_e));
	if (mt_exc_on() && FE_UNDERFLOW != 0)	/* FE_UNDERFLOW need not be defined (4.23) */
		pc_check((mt_fl & FE_UNDERFLOW) != 0, "%s: underflow, underflow exception not raised", what);
}

/* No error: errno untouched (it was set by MT_START_E), no error exception. */
static void mt_noerr(const char *what)
{
	pc_check(mt_e == EDOM, "%s changed errno from EDOM to %s", what, pc_errname(mt_e));
	if (mt_exc_on())
		pc_check((mt_fl & MT_ERRFLAGS) == 0, "%s raised an error exception (flags 0x%x)", what, mt_fl);
}

/* End of a test that judged error reports: needs one of the mechanisms. */
_Noreturn static void mt_finish_report(void)
{
	if (!mt_errno_on() && !mt_exc_on())
		pc_notinuse("math_errhandling selects neither errno nor exception flags");
	pc_pass();
}

#define MT_NEED_MX() do { if (!MT_MX) pc_notinuse("the IEC 60559 Floating-Point option is not claimed"); } while (0)
#define MT_NEED_NAN() do { if (MT_NO_NAN) pc_notinuse("quiet NaNs are not supported"); } while (0)

/* Rounding direction control. */
static int mt_enter(int i)
{
	if (mt_orig_mode < 0)
		mt_orig_mode = fegetround();
	if (fesetround(mt_modes[i]) != 0)
		return 0;
	mt_tried++;
	return 1;
}

static void mt_leave(void)
{
	if (mt_orig_mode >= 0)
		(void)fesetround(mt_orig_mode);
}

static void mt_need_modes(int n)
{
	mt_leave();
	if (mt_tried < n)
		pc_notinuse("fewer than %d rounding directions can be selected", n);
}

/* Values equal and carrying the same sign (so +0 and -0 differ). */
#define MT_SAME(r, v) ((r) == (v) && (signbit(r) != 0) == (signbit(v) != 0))

#endif
