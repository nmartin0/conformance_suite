/*
 * nl_langinfo(RADIXCHAR) and nl_langinfo(THOUSEP) give the decimal_point
 * and thousands_sep strings of the locale's LC_NUMERIC category.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.4-lc-numeric.004
 */
#include "pc.h"
#include "ldef.h"
#include <langinfo.h>

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];
	const char *r, *t;

	pc_begin("XBD.7.3.4-lc-numeric.004");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("numl", "cmap",
	    "LC_NUMERIC\ndecimal_point \"<comma>\"\nthousands_sep \"<period>\"\ngrouping 3\nEND LC_NUMERIC\n" REST, loc,
	    sizeof loc);
	pc_test();
	ld_load(loc);
	r = nl_langinfo(RADIXCHAR);
	t = nl_langinfo(THOUSEP);
	pc_check(r != NULL && strcmp(r, ",") == 0, "RADIXCHAR is \"%s\", not \",\"", r ? r : "(null)");
	pc_check(t != NULL && strcmp(t, ".") == 0, "THOUSEP is \"%s\", not \".\"", t ? t : "(null)");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
