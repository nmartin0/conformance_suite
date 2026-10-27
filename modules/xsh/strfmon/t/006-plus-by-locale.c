/*
 * In another locale the '+' flag fails exactly when both signs are empty.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.006
 */
#include "fmon.h"

int main(void)
{
	static const char *const fmt[2] = { "%+.0n", "%+.0i" };
	static const double val[2] = { 123.0, -123.0 };
	const struct lconv *lc;
	char buf[128];
	ssize_t r;
	int both, i, j, e;

	pc_begin("XSH.strfmon.006");
	lc_init();
	lc_need();
	if (setlocale(LC_ALL, lc_loc) == NULL)
		pc_unresolved("could not select the second locale");
	pc_test();
	lc = localeconv();
	both = lc->positive_sign[0] == '\0' && lc->negative_sign[0] == '\0';
	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++) {
			errno = 0;
			r = strfmon(buf, sizeof buf, fmt[i], val[j]);
			e = errno;
			if (both) {
				pc_check(r == -1 && e == EINVAL, "both signs are empty but strfmon(\"%s\", %g) returned %ld, errno %s", fmt[i], val[j], (long)r, pc_errname(e));
			} else {
				pc_check(r >= 0, "a sign is not empty but strfmon(\"%s\", %g) failed, errno %s", fmt[i], val[j], pc_errname(e));
				fm_want(fmt[i], val[j], "123");
			}
		}
	fm_want("%(.0n", -123.0, "123");
	fm_want("%(.0i", 123.0, "123");
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
