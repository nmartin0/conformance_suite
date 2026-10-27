/*
 * The '(' flag puts negative amounts in parentheses.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.007
 */
#include "fmon.h"

static void paren(const char *fmt, double v)
{
	char buf[128];
	size_t n;

	fm_ok(buf, sizeof buf, fmt, v);
	n = strlen(buf);
	pc_check(n >= 3 && buf[0] == '(' && buf[n - 1] == ')', "strfmon(\"%s\", %g) gave \"%s\", not enclosed in parentheses", fmt, v, buf);
	pc_check(strchr(buf, '-') == NULL, "strfmon(\"%s\", %g) gave \"%s\", which has a '-'", fmt, v, buf);
}

int main(void)
{
	char buf[128];

	pc_begin("XSH.strfmon.007");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	paren("%(.0n", -123.0);
	paren("%(.0i", -45.0);
	fm_want("%(.0n", -123.0, "123");
	fm_want("%(.0i", -45.0, "45");
	fm_ok(buf, sizeof buf, "%(.0n", 123.0);
	pc_check(strchr(buf, '(') == NULL && strchr(buf, ')') == NULL, "strfmon(\"%%(.0n\", 123.0) gave \"%s\", with parentheses", buf);
	fm_want("%(.0n", 123.0, "123");
	pc_pass();
}
