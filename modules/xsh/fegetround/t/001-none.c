/*
 * fegetround() is negative when no rounding direction macro exists.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetround.001
 */
#include "fh.h"

int main(void)
{
	int r;

	pc_begin("XSH.fegetround.001");
	if (FH_NMODES != 0)
		pc_notinuse("rounding direction macros are defined");
	pc_test();
	r = fegetround();
	pc_check(r < 0, "fegetround() returned %d, expected a negative value", r);
	pc_pass();
}
