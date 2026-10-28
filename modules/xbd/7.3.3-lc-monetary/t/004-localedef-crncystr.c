/*
 * nl_langinfo(CRNCYSTR) gives a minus sign followed by the currency
 * symbol when the symbol precedes the value, and a plus sign followed by
 * it when the symbol succeeds the value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.3-lc-monetary.004
 */
#include "pc.h"
#include "ldef.h"
#include <langinfo.h>

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char before[4200], after[4200];
	const char *s;

	pc_begin("XBD.7.3.3-lc-monetary.004");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("crb", "cmap",
	    "LC_MONETARY\ncurrency_symbol \"<Q><q>\"\np_cs_precedes 1\nn_cs_precedes 1\nEND LC_MONETARY\n" REST, before,
	    sizeof before);
	ld_ok("cra", "cmap",
	    "LC_MONETARY\ncurrency_symbol \"<Q><q>\"\np_cs_precedes 0\nn_cs_precedes 0\nEND LC_MONETARY\n" REST, after,
	    sizeof after);
	pc_test();
	ld_load(before);
	s = nl_langinfo(CRNCYSTR);
	pc_check(s != NULL && strcmp(s, "-Qq") == 0, "CRNCYSTR is \"%s\", not \"-Qq\", although the symbol precedes the value", s ? s : "(null)");
	ld_load(after);
	s = nl_langinfo(CRNCYSTR);
	pc_check(s != NULL && strcmp(s, "+Qq") == 0, "CRNCYSTR is \"%s\", not \"+Qq\", although the symbol succeeds the value", s ? s : "(null)");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
