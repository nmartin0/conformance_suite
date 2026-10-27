/*
 * The result including its NUL must fit in maxsize; no byte past maxsize is written.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.002
 */
#include "fmon.h"

static void guarded(const char *fmt, size_t m, long want, int err, double v)
{
	char buf[64];
	ssize_t r;
	size_t i;
	int e;

	memset(buf, 'X', sizeof buf);
	errno = 0;
	r = strfmon(buf, m, fmt, v);
	e = errno;
	pc_check(r == want, "strfmon(\"%s\", maxsize %zu) returned %ld, expected %ld", fmt, m, (long)r, want);
	if (err)
		pc_check(e == err, "strfmon(\"%s\", maxsize %zu) set errno to %s, expected %s", fmt, m, pc_errname(e), pc_errname(err));
	for (i = m; i < sizeof buf; i++)
		pc_check(buf[i] == 'X', "strfmon(\"%s\", maxsize %zu) wrote the byte at index %zu", fmt, m, i);
}

int main(void)
{
	char buf[64];
	ssize_t len;

	pc_begin("XSH.strfmon.002");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	guarded("abc", 4, 3, 0, 0.0);
	guarded("abc", 3, -1, E2BIG, 0.0);
	guarded("abc", 1, -1, E2BIG, 0.0);
	guarded("abc", 0, -1, E2BIG, 0.0);
	guarded("", 1, 0, 0, 0.0);
	guarded("", 0, -1, E2BIG, 0.0);
	len = strfmon(buf, sizeof buf, "%.0n", 123456.0);
	pc_check(len >= 6 && len < 60, "strfmon(\"%%.0n\", 123456.0) returned %ld", (long)len);
	guarded("%.0n", (size_t)len, -1, E2BIG, 123456.0);
	guarded("%.0n", (size_t)len + 1, len, 0, 123456.0);
	pc_pass();
}
