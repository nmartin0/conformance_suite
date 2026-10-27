/*
 * A failed fesetround() leaves the rounding direction unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetround.005
 */
#include "fh.h"

int main(void)
{
	int i, got, m0;

	pc_begin("XSH.fegetround.005");
	if (FH_NMODES == 0)
		pc_notinuse("no rounding direction macro is defined");
	m0 = fegetround();
	pc_test();
	for (i = 0; i < FH_NMODES; i++) {
		if (fesetround(fh_modes[i]) != 0)
			pc_unresolved("fesetround(%d) failed", fh_modes[i]);
		(void)fesetround(-1);
		got = fegetround();
		pc_check(got == fh_modes[i], "rounding direction became %d after fesetround(-1), expected it to stay %d", got, fh_modes[i]);
	}
	if (m0 >= 0)
		(void)fesetround(m0);
	pc_pass();
}
