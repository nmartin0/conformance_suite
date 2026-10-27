/*
 * FP_ILOGB0 and FP_ILOGBNAN have the allowed values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ilogb.008
 */
#include "mt.h"

int main(void)
{
	pc_begin("XSH.ilogb.008");
	pc_test();
	enum { zero_is_const = (FP_ILOGB0 == INT_MIN) || (FP_ILOGB0 == -INT_MAX), nan_is_const = (FP_ILOGBNAN == INT_MAX) || (FP_ILOGBNAN == INT_MIN) };
	volatile int z = FP_ILOGB0, n = FP_ILOGBNAN;

	pc_check(zero_is_const && (z == INT_MIN || z == -INT_MAX), "FP_ILOGB0 is %d", (int)z);
	pc_check(nan_is_const && (n == INT_MAX || n == INT_MIN), "FP_ILOGBNAN is %d", (int)n);
	pc_pass();
}
