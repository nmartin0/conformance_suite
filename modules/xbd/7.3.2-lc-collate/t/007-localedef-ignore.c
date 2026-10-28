/*
 * A character whose first weight is IGNORE is left out when the first
 * weights are compared but counts at the second weight: with the hyphen
 * ignored at the first level, a-c collates after ab, and a-b collates
 * before ab on the second weight.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.007
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];

	pc_begin("XBD.7.3.2-lc-collate.007");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("ign", "cmap",
	    "LC_COLLATE\norder_start forward;forward\n<hyphen-minus> IGNORE;<hyphen-minus>\n<a> <a>;<a>\n<b> <b>;<b>\n<c> <c>;<c>\n"
	    "UNDEFINED IGNORE;...\norder_end\nEND LC_COLLATE\n" REST,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	pc_check(strcoll("a-c", "ab") > 0, "a-c does not collate after ab: the hyphen is not ignored at the first weight");
	pc_check(strcoll("ab", "a-c") < 0, "ab does not collate before a-c");
	pc_check(strcoll("a-b", "ab") < 0 && strcoll("ab", "a-b") > 0,
	    "a-b and ab are not told apart by the second weight, on which the hyphen counts");
	pc_check(strcoll("ab", "ab") == 0 && strcoll("a-c", "ac") < 0, "ab with itself, or a-c against ac, collates wrongly");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
