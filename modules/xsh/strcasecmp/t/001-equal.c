/*
 * strcasecmp: equal ignoring case.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcasecmp.001
 */
#include "sc.h"
#include <stdio.h>

int main(void)
{
	pc_begin("XSH.strcasecmp.001");
	sc_init();
	pc_test();
	char la[96], ua[96];
	int i, j;
	char a[2], b[2];

	sc_expect(0, "abc", "ABC", 0, (locale_t)0, 0);
	sc_expect(0, "AbC", "aBc", 0, (locale_t)0, 0);
	sc_expect(0, "a1_", "A1_", 0, (locale_t)0, 0);
	sc_expect(0, "", "", 0, (locale_t)0, 0);
	for (i = 0; i < 26; i++)
		for (j = 0; j < 4; j++) {
			a[0] = (j & 1) ? sc_upper[i] : sc_lower[i];
			b[0] = (j & 2) ? sc_upper[i] : sc_lower[i];
			a[1] = b[1] = 0;
			sc_expect(0, a, b, 0, (locale_t)0, 0);
		}
	snprintf(la, sizeof la, "%s%s%s", sc_lower, sc_digit, sc_punct);
	snprintf(ua, sizeof ua, "%s%s%s", sc_upper, sc_digit, sc_punct);
	sc_expect(0, la, ua, 0, (locale_t)0, 0);
	sc_expect(0, ua, la, 0, (locale_t)0, 0);
	sc_expect(0, la, la, 0, (locale_t)0, 0);
	pc_pass();
}
