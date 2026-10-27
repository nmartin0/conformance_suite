/*
 * The default sign of a negative amount in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.004
 */
#include "fmon.h"

static void sign(const char *fmt)
{
	char buf[128], dg[128];
	const char *m, *d;
	size_t i;

	fm_ok(buf, sizeof buf, fmt, 123.0);
	pc_check(strchr(buf, '-') == NULL, "strfmon(\"%s\", 123.0) gave \"%s\", which has a '-'", fmt, buf);
	fm_digits(buf, dg, sizeof dg);
	pc_check(strcmp(dg, "123") == 0, "strfmon(\"%s\", 123.0) gave \"%s\", digits \"%s\"", fmt, buf, dg);

	fm_ok(buf, sizeof buf, fmt, -123.0);
	m = strchr(buf, '-');
	pc_check(m != NULL, "strfmon(\"%s\", -123.0) gave \"%s\", which has no '-'", fmt, buf);
	pc_check(strchr(m + 1, '-') == NULL, "strfmon(\"%s\", -123.0) gave \"%s\", which has more than one '-'", fmt, buf);
	for (i = 0; buf[i] != '\0' && !(buf[i] >= '0' && buf[i] <= '9'); i++)
		;
	d = buf + i;
	pc_check(m < d, "strfmon(\"%s\", -123.0) gave \"%s\", with the '-' after the digits", fmt, buf);
	fm_digits(buf, dg, sizeof dg);
	pc_check(strcmp(dg, "123") == 0, "strfmon(\"%s\", -123.0) gave \"%s\", digits \"%s\"", fmt, buf, dg);
}

int main(void)
{
	pc_begin("XSH.strfmon.004");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	sign("%.0n");
	sign("%!.0n");
	sign("%.0i");
	pc_pass();
}
