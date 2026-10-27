/*
 * The classification macros are distinct integer constants.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpclassify.006
 */
#include "mt.h"

int main(void)
{
	pc_begin("XSH.fpclassify.006");
	pc_test();
	volatile double x = 1.0;
	volatile int r = fpclassify(x);
	int known = 0;

	switch (r) {
	case FP_INFINITE:
	case FP_NAN:
	case FP_SUBNORMAL:
	case FP_ZERO:
		break;
	case FP_NORMAL:
		known = 1;
		break;
	default:
		break;
	}
	pc_check(known, "fpclassify(1.0) gave %d, not FP_NORMAL", r);
	pc_pass();
}
