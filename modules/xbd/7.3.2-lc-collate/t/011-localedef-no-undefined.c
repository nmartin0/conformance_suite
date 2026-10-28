/*
 * A collation order without UNDEFINED, in a character set that holds
 * characters it does not list, makes localedef issue a warning (exit
 * status 1); with -c the locale is made, and the unlisted characters
 * collate after the listed ones, in ascending order of encoded value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.011
 */
#include "pc.h"
#include "ldef.h"

static int sg(int v)
{
	return v < 0 ? -1 : v > 0;
}

/* sign of the difference of the encoded values of two portable characters */
static int ev(char a, char b)
{
	return (unsigned char)a < (unsigned char)b ? -1 : (unsigned char)a > (unsigned char)b;
}

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];
	int rc;

	pc_begin("XBD.7.3.2-lc-collate.011");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	pc_make_file("noundef.src", "LC_COLLATE\norder_start forward\n<b>\n<a>\norder_end\nEND LC_COLLATE\n" REST);
	ld_path(loc, sizeof loc, "noundef.loc");
	rc = ld_run("-c", "cmap", "noundef.src", loc);
	if (rc > 1) {
		ld_errnote();
		pc_unresolved("localedef refused the test source (status %d)", rc);
	}
	pc_test();
	pc_check(rc == 1, "localedef exited with status %d for a collation order that leaves characters unlisted and has no UNDEFINED; the status for a warning is 1", rc);
	pc_check(access(loc, F_OK) == 0, "localedef -c made no locale although only a warning was due");
	ld_load(loc);
	pc_check(strcoll("b", "a") < 0, "b does not collate before a");
	pc_check(strcoll("a", "c") < 0 && strcoll("a", "A") < 0 && strcoll("a", "0") < 0,
	    "an unlisted character does not collate after the listed ones");
	pc_check(sg(strcoll("0", "A")) == ev('0', 'A') && sg(strcoll("A", "c")) == ev('A', 'c') && strcoll("c", "d") < 0,
	    "the unlisted characters are not in ascending order of encoded value");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
