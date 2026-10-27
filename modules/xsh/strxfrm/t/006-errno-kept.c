/*
 * strxfrm() and strxfrm_l() leave errno alone on success.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strxfrm.006
 */
#include "lc.h"

int main(void)
{
	static const int before[2] = { 0, EDOM };
	char buf[128];
	locale_t obj;
	int i, e;

	pc_begin("XSH.strxfrm.006");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)strxfrm(buf, "abc", sizeof buf);
		e = errno;
		pc_check(e == before[i], "strxfrm() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)strxfrm_l(buf, "abc", sizeof buf, obj);
		e = errno;
		pc_check(e == before[i], "strxfrm_l() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	freelocale(obj);
	pc_pass();
}
