/*
 * towlower: round trip.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towlower.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t r;
	pc_begin("XSH.towlower.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		if (!wc_in(C_UPPER, i))
			continue;
		r = towlower(wc_ch[i]);
		pc_check(iswlower(r) != 0, "towlower() of the character at position %d is not a letter of the other case", i);
		pc_check(towupper(r) == wc_ch[i], "towupper(towlower()) did not return the original at position %d", i);
	}
	pc_pass();
}
