/*
 * strcasecmp: errno is not changed on valid input.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcasecmp.005
 */
#include "sc.h"

int main(void)
{
	locale_t loc;
	int e = EDOM, k;
	static const char *const s[4] = { "abc", "ABC", "abd", "ABB" };
	int i, j;

	pc_begin("XSH.strcasecmp.005");
	sc_init();
	loc = sc_cloc();
	pc_test();
	for (k = 0; k < 2; k++)
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++) {
				errno = e;
				(void)sc_cmp(k, s[i], s[j], 0, loc);
				pc_check(errno == e, "errno changed to %d by comparison kind %d of \"%s\" and \"%s\"", errno, k, s[i], s[j]);
			}
	freelocale(loc);
	pc_pass();
}
