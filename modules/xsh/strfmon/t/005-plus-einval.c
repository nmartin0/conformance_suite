/*
 * The '+' flag fails with EINVAL in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.005
 */
#include "fmon.h"

int main(void)
{
	static const char *const fmt[2] = { "%+.0n", "%+.0i" };
	static const double val[2] = { 123.0, -123.0 };
	char buf[64];
	ssize_t r;
	int i, j, e;

	pc_begin("XSH.strfmon.005");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++) {
			errno = 0;
			r = strfmon(buf, sizeof buf, fmt[i], val[j]);
			e = errno;
			pc_check(r == -1, "strfmon(\"%s\", %g) returned %ld, expected -1", fmt[i], val[j], (long)r);
			pc_check(e == EINVAL, "strfmon(\"%s\", %g) set errno to %s, expected EINVAL", fmt[i], val[j], pc_errname(e));
		}
	pc_pass();
}
