/*
 * EPSILON, MIN and MAX agree with MANT_DIG, MIN_EXP and MAX_EXP.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.float.h.003
 */
#include <float.h>
#include "pc.h"

#if FLT_MAX_EXP < FLT_MANT_DIG || DBL_MAX_EXP < DBL_MANT_DIG || LDBL_MAX_EXP < LDBL_MANT_DIG
#error "a MAX_EXP value is below the matching MANT_DIG value"
#endif

#if FLT_RADIX == 2 || FLT_RADIX == 4 || FLT_RADIX == 8 || FLT_RADIX == 16
/*
 * With a radix that is a power of two, division and multiplication by the
 * radix are exact, so the model values can be rebuilt and compared exactly.
 */
#define MODEL(T, P) \
static void model_##P(void) \
{ \
	volatile T eps = 1, mn = 1, mx; \
	int i; \
\
	for (i = 0; i < P##_MANT_DIG - 1; i++) \
		eps /= FLT_RADIX; \
	pc_check(eps == P##_EPSILON, #P "_EPSILON is not radix^(1-digits)"); \
	for (i = 0; i < 1 - P##_MIN_EXP; i++) \
		mn /= FLT_RADIX; \
	pc_check(mn == P##_MIN, #P "_MIN is not radix^(min_exp-1)"); \
	mx = 1 - P##_EPSILON / FLT_RADIX; \
	for (i = 0; i < P##_MAX_EXP; i++) \
		mx *= FLT_RADIX; \
	pc_check(mx == P##_MAX, #P "_MAX is not (1-radix^-digits)*radix^max_exp"); \
}
MODEL(float, FLT)
MODEL(double, DBL)
MODEL(long double, LDBL)
#define RUN_MODEL() (model_FLT(), model_DBL(), model_LDBL())
#else
#define RUN_MODEL() ((void)0)
#endif

int main(void)
{
	pc_begin("XBD.float.h.003");
	pc_test();
	RUN_MODEL();
	pc_pass();
}
