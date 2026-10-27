/*
 * fegetround() reports the direction that fesetround() established.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetround.003
 */
#include "fh.h"

int main(void)
{
	int i, r, m0;

	pc_begin("XSH.fegetround.003");
	if (FH_NMODES == 0)
		pc_notinuse("no rounding direction macro is defined");
	m0 = fegetround();
	pc_test();
	for (i = 0; i < FH_NMODES; i++) {
		if (fesetround(fh_modes[i]) != 0)
			pc_unresolved("fesetround(%d) failed", fh_modes[i]);
		r = fegetround();
		pc_check(r == fh_modes[i], "fegetround() returned %d after fesetround(%d)", r, fh_modes[i]);
	}
	if (m0 >= 0)
		(void)fesetround(m0);
	pc_pass();
}
