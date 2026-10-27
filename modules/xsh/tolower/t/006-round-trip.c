/*
 * tolower: round trip
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tolower.006
 */
#include "cv.h"

int main(void)
{
	locale_t loc = (locale_t)0;
	int i, via, m, r;

	pc_begin("XSH.tolower.006");
	ct_init();
	loc = ct_cloc();
	pc_test();
	for (via = 0; via <= 3; via++)
		for (i = 0; i < 26; i++) {
			m = cv_call(0, via, (unsigned char)ct_upper[i], loc);
			r = cv_call(1, via, m, loc);
			pc_check(r == (unsigned char)ct_upper[i], "toupper (%s) of the converted character code %d returned %d, expected %d",
				cv_via[via], m, r, (unsigned char)ct_upper[i]);
		}
	pc_pass();
}
