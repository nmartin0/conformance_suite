/*
 * toupper: round trip
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.toupper.006
 */
#include "cv.h"

int main(void)
{
	locale_t loc = (locale_t)0;
	int i, via, m, r;

	pc_begin("XSH.toupper.006");
	ct_init();
	loc = ct_cloc();
	pc_test();
	for (via = 0; via <= 3; via++)
		for (i = 0; i < 26; i++) {
			m = cv_call(1, via, (unsigned char)ct_lower[i], loc);
			r = cv_call(0, via, m, loc);
			pc_check(r == (unsigned char)ct_lower[i], "tolower (%s) of the converted character code %d returned %d, expected %d",
				cv_via[via], m, r, (unsigned char)ct_lower[i]);
		}
	pc_pass();
}
