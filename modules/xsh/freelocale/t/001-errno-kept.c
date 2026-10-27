/*
 * freelocale() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.freelocale.001
 */
#include "lc.h"

int main(void)
{
	static const int pre[2] = { 0, EDOM };
	locale_t o, r;
	int k, e;

	pc_begin("XSH.freelocale.001");
	lc_init();
	pc_test();
	for (k = 0; k < 2; k++) {
		o = newlocale(LC_ALL_MASK, "C", (locale_t)0);
		if (o == (locale_t)0)
			pc_unresolved("newlocale() could not make an object");
		errno = pre[k];
		freelocale(o);
		e = errno;
		pc_check(e == pre[k], "freelocale() of a newlocale() object changed errno from %s to %s", pc_errname(pre[k]), pc_errname(e));

		o = newlocale(LC_ALL_MASK, "C", (locale_t)0);
		if (o == (locale_t)0)
			pc_unresolved("newlocale() could not make an object");
		r = duplocale(o);
		if (r == (locale_t)0)
			pc_unresolved("duplocale() could not copy the object");
		freelocale(o);
		errno = pre[k];
		freelocale(r);
		e = errno;
		pc_check(e == pre[k], "freelocale() of a duplocale() object changed errno from %s to %s", pc_errname(pre[k]), pc_errname(e));

		o = newlocale(LC_CTYPE_MASK, "C", (locale_t)0);
		if (o == (locale_t)0)
			pc_unresolved("newlocale() could not make an object");
		r = newlocale(LC_TIME_MASK, "C", o);
		if (r == (locale_t)0)
			pc_unresolved("newlocale() could not modify the object");
		errno = pre[k];
		freelocale(r);
		e = errno;
		pc_check(e == pre[k], "freelocale() of an object made with a base changed errno from %s to %s", pc_errname(pre[k]), pc_errname(e));
	}
	pc_pass();
}
