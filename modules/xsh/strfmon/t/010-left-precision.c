/*
 * Left precision and the fill character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.010
 */
#include "fmon.h"

int main(void)
{
	char a[128];
	const char *m, *d;

	pc_begin("XSH.strfmon.010");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	fm_ok(a, sizeof a, "%=*#5.0n", 123.0);
	pc_check(strstr(a, "**123") != NULL && strstr(a, "***") == NULL, "strfmon(\"%%=*#5.0n\", 123.0) gave \"%s\", expected exactly two '*' before 123", a);
	fm_ok(a, sizeof a, "%=0#5.0n", 123.0);
	pc_check(strstr(a, "00123") != NULL && strstr(a, "000") == NULL, "strfmon(\"%%=0#5.0n\", 123.0) gave \"%s\", expected exactly two '0' before 123", a);
	fm_ok(a, sizeof a, "%=*#5.0n", -123.0);
	m = strchr(a, '-');
	d = strstr(a, "**123");
	pc_check(d != NULL && m != NULL && m < d, "strfmon(\"%%=*#5.0n\", -123.0) gave \"%s\", expected the '-' before \"**123\"", a);
	fm_ok(a, sizeof a, "%#5.0n", 123.0);
	pc_check(strstr(a, "  123") != NULL, "strfmon(\"%%#5.0n\", 123.0) gave \"%s\", expected two spaces before 123", a);
	fm_ok(a, sizeof a, "%=*#2.0n", 12345.0);
	pc_check(strchr(a, '*') == NULL, "strfmon(\"%%=*#2.0n\", 12345.0) gave \"%s\", with a fill character", a);
	fm_want("%=*#2.0n", 12345.0, "12345");
	fm_ok(a, sizeof a, "%=*.0n", 123.0);
	pc_check(strchr(a, '*') == NULL, "strfmon(\"%%=*.0n\", 123.0) gave \"%s\", with a fill character although no left precision was given", a);
	pc_pass();
}
