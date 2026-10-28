/*
 * The value a locale environment variable is given is the name operand of
 * the localedef run that made the locale; when it begins with a slash it is
 * the pathname of the locale definition: with LC_ALL naming such a
 * pathname, selecting the locale from the environment succeeds and the
 * locale's own data are in effect.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.1-locale-general.001
 */
#include "pc.h"
#include "ldef.h"

#define SRC LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_TIME LD_CP_MESSAGES \
	"LC_NUMERIC\ndecimal_point \",\"\nthousands_sep \".\"\ngrouping 3;3\nEND LC_NUMERIC\n"

int main(void)
{
	char loc[4200];
	const struct lconv *lc;

	pc_begin("XBD.7.1-locale-general.001");
	ld_need();
	ld_ok("pathloc", NULL, SRC, loc, sizeof loc);
	ld_unset_all();
	pc_check(setenv("LC_ALL", loc, 1) == 0, "setenv failed");
	pc_test();
	pc_check(setlocale(LC_ALL, "") != NULL, "setlocale(LC_ALL, \"\") failed with LC_ALL naming the locale pathname %s", loc);
	lc = localeconv();
	pc_check(strcmp(lc->decimal_point, ",") == 0, "the radix character is \"%s\", not the \",\" the locale defines", lc->decimal_point);
	pc_check(strcmp(lc->thousands_sep, ".") == 0, "the grouping separator is \"%s\", not the \".\" the locale defines",
	    lc->thousands_sep);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
