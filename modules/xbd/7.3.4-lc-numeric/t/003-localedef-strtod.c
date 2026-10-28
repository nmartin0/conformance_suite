/*
 * strtod() reads the radix of the locale as the decimal point, and no
 * longer reads the period when the radix is a comma; the thousands
 * separator is not part of a number.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.4-lc-numeric.003
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];
	const char *s;
	char *end;
	double d;

	pc_begin("XBD.7.3.4-lc-numeric.003");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("numr", "cmap",
	    "LC_NUMERIC\ndecimal_point \"<comma>\"\nthousands_sep \"<period>\"\ngrouping 3\nEND LC_NUMERIC\n" REST, loc,
	    sizeof loc);
	pc_test();
	ld_load(loc);
	s = "2,5x";
	d = strtod(s, &end);
	pc_check(d > 2.49 && d < 2.51 && end == s + 3, "strtod of \"2,5x\" gave %g and consumed %ld bytes, not 2.5 and 3", d, (long)(end - s));
	s = "2.5x";
	d = strtod(s, &end);
	pc_check(d > 1.99 && d < 2.01 && end == s + 1, "strtod of \"2.5x\" gave %g and consumed %ld bytes, not 2 and 1", d, (long)(end - s));
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
