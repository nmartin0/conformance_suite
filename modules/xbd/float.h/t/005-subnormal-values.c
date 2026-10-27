/*
 * HAS_SUBNORM values and their relation to TRUE_MIN and MIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.float.h.005
 */
#include <float.h>
#include "pc.h"

#if FLT_HAS_SUBNORM < -1 || FLT_HAS_SUBNORM > 1 || DBL_HAS_SUBNORM < -1 || DBL_HAS_SUBNORM > 1 || LDBL_HAS_SUBNORM < -1 || LDBL_HAS_SUBNORM > 1
#error "a HAS_SUBNORM value is not -1, 0 or 1"
#endif

int main(void)
{
	pc_begin("XBD.float.h.005");
	pc_test();
	pc_check((long double)FLT_TRUE_MIN <= (long double)FLT_MIN, "FLT_TRUE_MIN is above FLT_MIN");
	pc_check((long double)DBL_TRUE_MIN <= (long double)DBL_MIN, "DBL_TRUE_MIN is above DBL_MIN");
	pc_check((long double)LDBL_TRUE_MIN <= (long double)LDBL_MIN, "LDBL_TRUE_MIN is above LDBL_MIN");
#if FLT_HAS_SUBNORM == 1
	pc_check((long double)FLT_TRUE_MIN < (long double)FLT_MIN, "FLT_HAS_SUBNORM is 1 but FLT_TRUE_MIN is not below FLT_MIN");
#endif
#if DBL_HAS_SUBNORM == 1
	pc_check((long double)DBL_TRUE_MIN < (long double)DBL_MIN, "DBL_HAS_SUBNORM is 1 but DBL_TRUE_MIN is not below DBL_MIN");
#endif
#if LDBL_HAS_SUBNORM == 1
	pc_check((long double)LDBL_TRUE_MIN < (long double)LDBL_MIN, "LDBL_HAS_SUBNORM is 1 but LDBL_TRUE_MIN is not below LDBL_MIN");
#endif
	pc_pass();
}
