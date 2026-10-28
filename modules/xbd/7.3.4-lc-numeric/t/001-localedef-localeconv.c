/*
 * The LC_NUMERIC keywords decimal_point, thousands_sep and grouping given
 * to localedef reach the members of the localeconv() result that bear
 * their names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.4-lc-numeric.001
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];
	const struct lconv *lc;

	pc_begin("XBD.7.3.4-lc-numeric.001");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("num", "cmap",
	    "LC_NUMERIC\ndecimal_point \"<comma>\"\nthousands_sep \"<period>\"\ngrouping 3;2\nEND LC_NUMERIC\n" REST, loc,
	    sizeof loc);
	pc_test();
	ld_load(loc);
	lc = localeconv();
	pc_check(lc != NULL, "localeconv returned a null pointer");
	pc_check(strcmp(lc->decimal_point, ",") == 0, "decimal_point is \"%s\", not \",\"", lc->decimal_point);
	pc_check(strcmp(lc->thousands_sep, ".") == 0, "thousands_sep is \"%s\", not \".\"", lc->thousands_sep);
	pc_check(strcmp(lc->grouping, "\3\2") == 0, "grouping is not the groups 3 and 2");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
