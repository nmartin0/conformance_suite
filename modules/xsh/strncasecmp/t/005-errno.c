/*
 * strncasecmp: errno is not changed on valid input.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncasecmp.005
 */
#include "sc.h"

int main(void)
{
	locale_t loc;
	int e = EDOM, k;
	static const char *const s[4] = { "abc", "ABC", "abd", "ABB" };
	static const size_t ns[3] = { 0, 2, 10 };
	int i, j, m;

	pc_begin("XSH.strncasecmp.005");
	sc_init();
	loc = sc_cloc();
	pc_test();
	for (k = 2; k < 4; k++)
		for (m = 0; m < 3; m++)
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++) {
					errno = e;
					(void)sc_cmp(k, s[i], s[j], ns[m], loc);
					pc_check(errno == e, "errno changed to %d by comparison kind %d of \"%s\" and \"%s\" (n %d)", errno, k, s[i], s[j], (int)ns[m]);
				}
	freelocale(loc);
	pc_pass();
}
