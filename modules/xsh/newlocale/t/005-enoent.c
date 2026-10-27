/*
 * A missing locale gives ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.newlocale.005
 */
#include "lc.h"

int main(void)
{
	locale_t r;
	int e;

	pc_begin("XSH.newlocale.005");
	lc_init();
	lc_nosuch();
	pc_test();
	errno = 0;
	r = newlocale(LC_ALL_MASK, LC_NOSUCH, (locale_t)0);
	e = errno;
	if (r != (locale_t)0) {
		freelocale(r);
		pc_unresolved("newlocale() accepted the made-up locale name");
	}
	pc_check(e == ENOENT, "newlocale() with a missing locale set errno to %s, expected ENOENT", pc_errname(e));
	errno = 0;
	r = newlocale(LC_TIME_MASK, LC_NOSUCH, (locale_t)0);
	e = errno;
	if (r != (locale_t)0) {
		freelocale(r);
		pc_unresolved("newlocale() accepted the made-up locale name for LC_TIME");
	}
	pc_check(e == ENOENT, "newlocale(LC_TIME_MASK, ...) with a missing locale set errno to %s, expected ENOENT", pc_errname(e));
	pc_pass();
}
