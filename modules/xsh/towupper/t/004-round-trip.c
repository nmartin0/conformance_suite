/*
 * towupper: round trip.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towupper.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t r;
	pc_begin("XSH.towupper.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		if (!wc_in(C_LOWER, i))
			continue;
		r = towupper(wc_ch[i]);
		pc_check(iswupper(r) != 0, "towupper() of the character at position %d is not a letter of the other case", i);
		pc_check(towlower(r) == wc_ch[i], "towlower(towupper()) did not return the original at position %d", i);
	}
	pc_pass();
}
