/*
 * Field width and the '-' flag.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.009
 */
#include "fmon.h"

int main(void)
{
	char a[128], b[128];

	pc_begin("XSH.strfmon.009");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	fm_ok(a, sizeof a, "%10.0n", 123.0);
	pc_check(strlen(a) == 10, "strfmon(\"%%10.0n\", 123.0) gave \"%s\", of length %zu, expected 10", a, strlen(a));
	pc_check(a[0] == ' ', "strfmon(\"%%10.0n\", 123.0) gave \"%s\", not right-justified", a);
	fm_want("%10.0n", 123.0, "123");

	fm_ok(a, sizeof a, "%-10.0n", 123.0);
	pc_check(strlen(a) == 10, "strfmon(\"%%-10.0n\", 123.0) gave \"%s\", of length %zu, expected 10", a, strlen(a));
	pc_check(a[strlen(a) - 1] == ' ', "strfmon(\"%%-10.0n\", 123.0) gave \"%s\", not left-justified", a);
	fm_want("%-10.0n", 123.0, "123");

	fm_ok(a, sizeof a, "%=*10#5.0n", 123.0);
	pc_check(strlen(a) == 10 && strstr(a, "**123") != NULL && a[0] == ' ', "strfmon(\"%%=*10#5.0n\", 123.0) gave \"%s\", expected 10 bytes with a space as the width fill and '*' only as the digit fill", a);

	fm_ok(a, sizeof a, "%2.0n", 123456.0);
	pc_check(strstr(a, "123456") != NULL, "strfmon(\"%%2.0n\", 123456.0) gave \"%s\", the amount was cut", a);

	fm_ok(a, sizeof a, "%-.0n", 123.0);
	fm_ok(b, sizeof b, "%.0n", 123.0);
	pc_check(strcmp(a, b) == 0, "'-' without a width changed the result: \"%s\" and \"%s\"", a, b);
	pc_pass();
}
