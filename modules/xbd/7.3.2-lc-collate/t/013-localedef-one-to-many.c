/*
 * A weight written as two concatenated characters maps one character to
 * two collating elements: here x has the weight "<a><a>", so x collates
 * as aa (equal to aa, after a, before ab); an empty first operand stands
 * for the collating element itself.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.013
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200], emp[4200];

	pc_begin("XBD.7.3.2-lc-collate.013");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("o2m", "cmap",
	    "LC_COLLATE\norder_start forward\n<a> <a>\n<b> <b>\n<x> \"<a><a>\"\nUNDEFINED\norder_end\nEND LC_COLLATE\n" REST, loc,
	    sizeof loc);
	ld_ok("empw", "cmap",
	    "LC_COLLATE\norder_start forward;forward\n<b> ;<b>\n<a> ;<a>\nUNDEFINED\norder_end\nEND LC_COLLATE\n" REST, emp, sizeof emp);
	pc_test();
	ld_load(loc);
	pc_check(strcoll("x", "aa") == 0, "x does not collate as equal to aa");
	pc_check(strcoll("a", "x") < 0, "a does not collate before x");
	pc_check(strcoll("x", "ab") < 0, "x does not collate before ab");
	pc_check(strcoll("xb", "aab") == 0, "xb does not collate as equal to aab");
	ld_load(emp);
	pc_check(strcoll("b", "a") < 0, "b does not collate before a although an empty first operand stands for the element itself");
	pc_check(strcoll("ba", "ab") < 0, "ba does not collate before ab");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
