/*
 * strcoll() and strcoll_l() leave errno alone on success.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcoll.005
 */
#include "coll.h"

/* The results are kept so that the calls cannot be dropped as unused. */
static volatile int sink;

int main(void)
{
	static const int before[2] = { 0, EDOM };
	locale_t obj;
	int i, e;

	pc_begin("XSH.strcoll.005");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		sink = strcoll("ab", "ac");
		e = errno;
		pc_check(e == before[i], "strcoll() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		sink = strcoll_l("ab", "ac", obj);
		e = errno;
		pc_check(e == before[i], "strcoll_l() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	freelocale(obj);
	pc_pass();
}
