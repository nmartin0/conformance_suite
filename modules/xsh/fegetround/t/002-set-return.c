/*
 * fesetround() returns zero for each rounding direction macro.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetround.002
 */
#include "fh.h"

int main(void)
{
	int i, r, m0;

	pc_begin("XSH.fegetround.002");
	if (FH_NMODES == 0)
		pc_notinuse("no rounding direction macro is defined");
	m0 = fegetround();
	pc_test();
	for (i = 0; i < FH_NMODES; i++) {
		r = fesetround(fh_modes[i]);
		pc_check(r == 0, "fesetround(%d) returned %d, expected 0", fh_modes[i], r);
	}
	if (m0 >= 0)
		(void)fesetround(m0);
	pc_pass();
}
