/*
 * newlocale() for "C" and "POSIX" makes a POSIX-locale object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.newlocale.001
 */
#include "lc.h"

int main(void)
{
	static const char *const nm[2] = { "C", "POSIX" };
	locale_t obj;
	int k;

	pc_begin("XSH.newlocale.001");
	lc_init();
	pc_test();
	for (k = 0; k < 2; k++) {
		obj = newlocale(LC_ALL_MASK, nm[k], (locale_t)0);
		pc_check(obj != (locale_t)0, "newlocale(LC_ALL_MASK, \"%s\", 0) failed, errno %s", nm[k], pc_errname(errno));
		lc_check_items(obj, nm[k]);
		freelocale(obj);
	}
	pc_pass();
}
