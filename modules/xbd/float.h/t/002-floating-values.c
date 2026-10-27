/*
 * Floating values of float.h are constants within the stated bounds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.float.h.002
 */
#include <float.h>
#include "pc.h"

#define POS(x) ((long double)(x) > 0)

static const float fmax_ = FLT_MAX, feps = FLT_EPSILON, fmin_ = FLT_MIN, ftmin = FLT_TRUE_MIN;
static const double dmax_ = DBL_MAX, deps = DBL_EPSILON, dmin_ = DBL_MIN, dtmin = DBL_TRUE_MIN;
static const long double lmax_ = LDBL_MAX, leps = LDBL_EPSILON, lmin_ = LDBL_MIN, ltmin = LDBL_TRUE_MIN;

int main(void)
{
	pc_begin("XBD.float.h.002");
	pc_test();
	pc_check(fmax_ >= 1E+37F && dmax_ >= 1E+37 && lmax_ >= 1E+37L, "a MAX value is below 1E+37");
	pc_check(POS(feps) && feps <= 1E-5F, "FLT_EPSILON is not positive and at most 1E-5");
	pc_check(POS(deps) && deps <= 1E-9, "DBL_EPSILON is not positive and at most 1E-9");
	pc_check(POS(leps) && leps <= 1E-9L, "LDBL_EPSILON is not positive and at most 1E-9");
	pc_check(POS(fmin_) && fmin_ <= 1E-37F, "FLT_MIN is not positive and at most 1E-37");
	pc_check(POS(dmin_) && dmin_ <= 1E-37, "DBL_MIN is not positive and at most 1E-37");
	pc_check(POS(lmin_) && lmin_ <= 1E-37L, "LDBL_MIN is not positive and at most 1E-37");
	pc_check(POS(ftmin) && ftmin <= 1E-37F, "FLT_TRUE_MIN is not positive and at most 1E-37");
	pc_check(POS(dtmin) && dtmin <= 1E-37, "DBL_TRUE_MIN is not positive and at most 1E-37");
	pc_check(POS(ltmin) && ltmin <= 1E-37L, "LDBL_TRUE_MIN is not positive and at most 1E-37");
	pc_pass();
}
