/*
 * Integer values of float.h: usability in #if and minimum magnitudes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.float.h.001
 */
#include <float.h>
#include "pc.h"
/* Each integer value is suitable for #if (FLT_ROUNDS excepted). */
#if 1 \
	&& ((FLT_RADIX) == (FLT_RADIX)) \
	&& ((DECIMAL_DIG) == (DECIMAL_DIG)) \
	&& ((FLT_EVAL_METHOD) == (FLT_EVAL_METHOD)) \
	&& ((FLT_MANT_DIG) == (FLT_MANT_DIG)) \
	&& ((FLT_DECIMAL_DIG) == (FLT_DECIMAL_DIG)) \
	&& ((FLT_DIG) == (FLT_DIG)) \
	&& ((FLT_MIN_EXP) == (FLT_MIN_EXP)) \
	&& ((FLT_MIN_10_EXP) == (FLT_MIN_10_EXP)) \
	&& ((FLT_MAX_EXP) == (FLT_MAX_EXP)) \
	&& ((FLT_MAX_10_EXP) == (FLT_MAX_10_EXP)) \
	&& ((FLT_HAS_SUBNORM) == (FLT_HAS_SUBNORM)) \
	&& ((DBL_MANT_DIG) == (DBL_MANT_DIG)) \
	&& ((DBL_DECIMAL_DIG) == (DBL_DECIMAL_DIG)) \
	&& ((DBL_DIG) == (DBL_DIG)) \
	&& ((DBL_MIN_EXP) == (DBL_MIN_EXP)) \
	&& ((DBL_MIN_10_EXP) == (DBL_MIN_10_EXP)) \
	&& ((DBL_MAX_EXP) == (DBL_MAX_EXP)) \
	&& ((DBL_MAX_10_EXP) == (DBL_MAX_10_EXP)) \
	&& ((DBL_HAS_SUBNORM) == (DBL_HAS_SUBNORM)) \
	&& ((LDBL_MANT_DIG) == (LDBL_MANT_DIG)) \
	&& ((LDBL_DECIMAL_DIG) == (LDBL_DECIMAL_DIG)) \
	&& ((LDBL_DIG) == (LDBL_DIG)) \
	&& ((LDBL_MIN_EXP) == (LDBL_MIN_EXP)) \
	&& ((LDBL_MIN_10_EXP) == (LDBL_MIN_10_EXP)) \
	&& ((LDBL_MAX_EXP) == (LDBL_MAX_EXP)) \
	&& ((LDBL_MAX_10_EXP) == (LDBL_MAX_10_EXP)) \
	&& ((LDBL_HAS_SUBNORM) == (LDBL_HAS_SUBNORM)) \
	
#endif

#if FLT_RADIX < 2
#error "FLT_RADIX is below 2"
#endif
#if FLT_DIG < 6 || DBL_DIG < 10 || LDBL_DIG < 10
#error "FLT_DIG, DBL_DIG or LDBL_DIG is below its minimum"
#endif
#if FLT_DECIMAL_DIG < 6 || DBL_DECIMAL_DIG < 10 || LDBL_DECIMAL_DIG < 10 || DECIMAL_DIG < 10
#error "FLT_DECIMAL_DIG, DBL_DECIMAL_DIG, LDBL_DECIMAL_DIG or DECIMAL_DIG is below its minimum"
#endif
#if FLT_MIN_10_EXP > -37 || DBL_MIN_10_EXP > -37 || LDBL_MIN_10_EXP > -37
#error "a MIN_10_EXP value is not -37 or less"
#endif
#if FLT_MAX_10_EXP < 37 || DBL_MAX_10_EXP < 37 || LDBL_MAX_10_EXP < 37
#error "a MAX_10_EXP value is not 37 or more"
#endif
#if FLT_MIN_EXP >= 0 || DBL_MIN_EXP >= 0 || LDBL_MIN_EXP >= 0
#error "a MIN_EXP value is not negative"
#endif
#if FLT_MAX_EXP <= 0 || DBL_MAX_EXP <= 0 || LDBL_MAX_EXP <= 0
#error "a MAX_EXP value is not positive"
#endif

int main(void)
{
	pc_begin("XBD.float.h.001");
	pc_test();
	pc_pass();
}
