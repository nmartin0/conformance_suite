/*
 * fh.h - helpers shared by the tests of one floating-point environment module.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Every FE_* macro is optional, so everything that names one goes through
 * the FH_* names below; an absent macro becomes 0 and is left out of the
 * lists. Exceptions are provoked by arithmetic on volatile operands.
 */
#ifndef FH_H
#define FH_H

#include "pc.h"
#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stddef.h>

#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT OFF

#ifdef FE_DIVBYZERO
#define FH_DIV FE_DIVBYZERO
#else
#define FH_DIV 0
#endif
#ifdef FE_INVALID
#define FH_INV FE_INVALID
#else
#define FH_INV 0
#endif
#ifdef FE_INEXACT
#define FH_INX FE_INEXACT
#else
#define FH_INX 0
#endif
#ifdef FE_OVERFLOW
#define FH_OVF FE_OVERFLOW
#else
#define FH_OVF 0
#endif
#ifdef FE_UNDERFLOW
#define FH_UNF FE_UNDERFLOW
#else
#define FH_UNF 0
#endif

/* Exceptions the implementation defines (zero-terminated). */
static const int fh_exc[] = {
#ifdef FE_DIVBYZERO
	FE_DIVBYZERO,
#endif
#ifdef FE_INVALID
	FE_INVALID,
#endif
#ifdef FE_INEXACT
	FE_INEXACT,
#endif
#ifdef FE_OVERFLOW
	FE_OVERFLOW,
#endif
#ifdef FE_UNDERFLOW
	FE_UNDERFLOW,
#endif
	0
};
#define FH_NEXC ((int)(sizeof fh_exc / sizeof fh_exc[0]) - 1)

/* Rounding directions the implementation defines (zero-terminated is not
 * possible, 0 is a legal value; -1 ends the list). */
static const int fh_modes[] = {
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
#define FH_NMODES ((int)(sizeof fh_modes / sizeof fh_modes[0]) - 1)

/* Provoke exception e by arithmetic; 0 when e is not one of the five. */
static inline int fh_arith(int e)
{
	volatile double a, b, r;

	if (e == 0)
		return 0;
	if (e == FH_DIV) { a = 1.0; b = 0.0; r = a / b; }
	else if (e == FH_INV) { a = 0.0; b = 0.0; r = a / b; }
	else if (e == FH_INX) { a = 1.0; b = 3.0; r = a / b; }
	else if (e == FH_OVF) { a = DBL_MAX; b = 2.0; r = a * b; }
	else if (e == FH_UNF) { a = DBL_MIN; b = 3.0; r = a / b; }
	else
		return 0;
	(void)r;
	return 1;
}

/* Setup: raise e by arithmetic; if its flag does not show, UNRESOLVED. */
static inline void fh_raise(int e)
{
	if (!fh_arith(e) || (fetestexcept(e) & e) == 0)
		pc_unresolved("could not raise exception 0x%x by arithmetic", e);
}

/* Setup: clear every flag. */
static inline void fh_clearall(void)
{
	if (FE_ALL_EXCEPT != 0 && feclearexcept(FE_ALL_EXCEPT) != 0)
		pc_unresolved("could not clear the exception flags");
}

/* Two exceptions that do not raise each other; else NOTINUSE. */
static int fh_e1, fh_e2;
static inline void fh_pair(void)
{
	if (FH_NEXC < 2)
		pc_notinuse("fewer than two exception macros are defined");
	fh_e1 = fh_exc[0];
	fh_e2 = fh_exc[1];
	if (fh_e1 == FH_INX && (fh_e2 == FH_OVF || fh_e2 == FH_UNF))
		pc_notinuse("no pair of independent exceptions is available");
}

/*
 * Two rounding directions that can be selected: *m0 is the current one,
 * *m1 another. Needs at least two macros, else NOTINUSE; a current mode
 * that cannot be determined is UNRESOLVED.
 */
static inline void fh_two_modes(int *m0, int *m1)
{
	int i;

	*m0 = fegetround();
	if (*m0 < 0)
		pc_unresolved("the current rounding direction cannot be determined");
	*m1 = -1;
	for (i = 0; i < FH_NMODES; i++) {
		if (fh_modes[i] == *m0)
			continue;
		if (fesetround(fh_modes[i]) == 0) {
			*m1 = fh_modes[i];
			break;
		}
	}
	(void)fesetround(*m0);
	if (*m1 < 0)
		pc_notinuse("fewer than two rounding directions can be selected");
}

#endif
