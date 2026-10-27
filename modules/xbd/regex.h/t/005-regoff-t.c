/*
 * regoff_t is a signed integer type that holds the largest ptrdiff_t and
 *   ssize_t values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.regex.h.005
 */
#include <regex.h>
#include <stdint.h>
#include <limits.h>
#include "pc.h"


int main(void)
{
	regoff_t r;

	pc_begin("XBD.regex.h.005");
	pc_test();
	pc_check((regoff_t)1.9 == 1, "regoff_t is not an integer type");
	pc_check((regoff_t)-1 < 0, "regoff_t is not signed");
	r = PTRDIFF_MAX;
	pc_check(r == PTRDIFF_MAX, "regoff_t cannot hold PTRDIFF_MAX");
	r = SSIZE_MAX;
	pc_check(r == SSIZE_MAX, "regoff_t cannot hold SSIZE_MAX");
	pc_pass();
}
