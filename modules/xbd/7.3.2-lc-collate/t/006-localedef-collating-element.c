/*
 * A multi-character collating element collates as one unit at the place
 * where it is listed (here after c and before d), not as its characters
 * one by one; a bracket expression can name it as a collating symbol and
 * cannot name a string that is not an element.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.006
 */
#include "pc.h"
#include "ldef.h"
#include <regex.h>

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];
	regex_t re;
	int r;

	pc_begin("XBD.7.3.2-lc-collate.006");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("elem", "cmap",
	    "LC_COLLATE\ncollating-element <ch> from \"<c><h>\"\norder_start forward\n<a>\n<b>\n<c>\n<ch>\n<d>\nUNDEFINED\n"
	    "order_end\nEND LC_COLLATE\n" REST,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	pc_check(strcoll("ch", "cz") > 0, "ch does not collate after cz: it is treated as c then h, not as one element after c");
	pc_check(strcoll("ach", "acz") > 0, "ach does not collate after acz");
	pc_check(strcoll("c", "ch") < 0 && strcoll("ch", "d") < 0, "the element ch is not between c and d");
	pc_check(strcoll("cha", "chb") < 0, "the characters after the element are not compared in order");
	r = regcomp(&re, "[[.ch.]]", REG_EXTENDED);
	pc_check(r == 0, "regcomp did not accept the collating symbol ch although the locale defines that element (%d)", r);
	regfree(&re);
	r = regcomp(&re, "[[.cx.]]", REG_EXTENDED);
	if (r == 0)
		regfree(&re);
	pc_check(r != 0, "regcomp accepted the collating symbol cx although the locale has no such element");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
