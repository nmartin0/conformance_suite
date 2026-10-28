/*
 * A collation sequence that does not order all characters totally makes
 * localedef warn and exit with status 1; with -c the locale is made, and
 * the two characters that share their only weight collate as equal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.008
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];
	int rc;

	pc_begin("XBD.7.3.2-lc-collate.008");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	pc_make_file("tie.src", "LC_COLLATE\norder_start forward\n<a>\n<A> <a>\nUNDEFINED\norder_end\nEND LC_COLLATE\n" REST);
	ld_path(loc, sizeof loc, "tie.loc");
	rc = ld_run("-c", "cmap", "tie.src", loc);
	if (rc > 1) {
		ld_errnote();
		pc_unresolved("localedef refused the test source (status %d)", rc);
	}
	pc_test();
	pc_check(rc == 1, "localedef exited with status %d for a collation sequence in which a and A are equal; the status for a warning is 1", rc);
	pc_check(access(loc, F_OK) == 0, "localedef -c made no locale although only a warning was due");
	ld_load(loc);
	pc_check(strcoll("a", "A") == 0 && strcoll("A", "a") == 0, "a and A, which share their only weight, do not collate as equal");
	pc_check(strcoll("a", "b") < 0, "a does not collate before the unlisted b");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
