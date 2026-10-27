/*
 * DIG, DECIMAL_DIG, MIN_10_EXP and MAX_10_EXP agree with the formulas.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.float.h.004
 */
#include <float.h>
#include "pc.h"

#if FLT_RADIX == 2 || FLT_RADIX == 4 || FLT_RADIX == 8 || FLT_RADIX == 16
#define LOG10_2 0.30102999566398119521373889472449302677L

static long double lfloor(long double x)
{
	long long i = (long long)x;

	return (long double)i > x ? i - 1 : i;
}

static long double lceil(long double x)
{
	long long i = (long long)x;

	return (long double)i < x ? i + 1 : i;
}

/* log10 of the radix, from log10(2) */
static long double lb(void)
{
	return (FLT_RADIX == 2 ? 1 : FLT_RADIX == 4 ? 2 : FLT_RADIX == 8 ? 3 : 4) * LOG10_2;
}

#define DIGITS(P) \
static void digits_##P(void) \
{ \
	long double l = lb(); \
\
	pc_check(P##_DIG == (int)lfloor((P##_MANT_DIG - 1) * l), #P "_DIG is not floor((p-1)*log10(radix))"); \
	pc_check(P##_DECIMAL_DIG == (int)lceil(1 + P##_MANT_DIG * l), #P "_DECIMAL_DIG is not ceil(1+p*log10(radix))"); \
	pc_check(P##_MIN_10_EXP == (int)lceil((P##_MIN_EXP - 1) * l), #P "_MIN_10_EXP is not ceil(log10(radix^(min_exp-1)))"); \
	/* the (1 - radix^-p) factor lowers the value by far less than 1E-6 */ \
	pc_check(P##_MAX_10_EXP == (int)lfloor(P##_MAX_EXP * l - 1E-6L), #P "_MAX_10_EXP is not floor(log10((1-radix^-p)*radix^max_exp))"); \
}
DIGITS(FLT)
DIGITS(DBL)
DIGITS(LDBL)
#define RUN_DIGITS() do { \
	digits_FLT(); digits_DBL(); digits_LDBL(); \
	pc_check(DECIMAL_DIG >= (int)lceil(1 + LDBL_MANT_DIG * lb()), "DECIMAL_DIG is below the value for long double"); \
} while (0)
#else
#define RUN_DIGITS() ((void)0)
#endif

int main(void)
{
	pc_begin("XBD.float.h.004");
	pc_test();
	RUN_DIGITS();
	pc_pass();
}
