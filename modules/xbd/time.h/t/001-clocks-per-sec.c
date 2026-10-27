/*
 * CLOCKS_PER_SEC has type clock_t and is positive.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.001
 */
#include <time.h>
#include "pc.h"

_Static_assert(_Generic(CLOCKS_PER_SEC, clock_t: 1, default: 0), "CLOCKS_PER_SEC does not have type clock_t");

int main(void)
{
	pc_begin("XBD.time.h.001");
	pc_test();
	pc_check(CLOCKS_PER_SEC > 0, "CLOCKS_PER_SEC is not positive");
	pc_pass();
}
