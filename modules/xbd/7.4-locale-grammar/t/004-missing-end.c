/*
 * A category that is not closed by its END statement, or is closed by the
 * END of a different category, is a syntax error: localedef exits with a
 * non-zero status. A well-formed source of the same categories is
 * accepted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.4-locale-grammar.004
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_TIME LD_CP_MESSAGES
#define NUM "LC_NUMERIC\ndecimal_point \"<comma>\"\nthousands_sep \"<period>\"\ngrouping 3\n"

static void bad(const char *tag, const char *what, const char *text)
{
	char loc[4200];
	int rc;

	pc_make_file(tag, text);
	ld_path(loc, sizeof loc, "bad.loc");
	rc = ld_run("-c", "cmap", tag, loc);
	if (rc == 255)
		pc_unresolved("localedef could not be run");
	pc_check(rc != 0, "localedef accepted a source with %s", what);
}

int main(void)
{
	char ok[4200];

	pc_begin("XBD.7.4-locale-grammar.004");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("good", "cmap", NUM "END LC_NUMERIC\n" REST, ok, sizeof ok);
	pc_test();
	bad("noend.src", "a category lacking its END statement before the next category", NUM REST);
	bad("noend2.src", "a final category lacking its END statement", REST NUM);
	bad("wrongend.src", "a category closed by the END of another category", NUM "END LC_TIME\n" REST);
	ld_load(ok);
	pc_check(strcmp(localeconv()->decimal_point, ",") == 0, "the well-formed control source was not made usable");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
