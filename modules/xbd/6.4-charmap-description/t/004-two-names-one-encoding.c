/*
 * A coded character value can have two symbolic names: the locale source
 * can use either name for the character, here one name in the lower list
 * and the other in a case mapping.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.4-charmap-description.004
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>

int main(void)
{
	char loc[4200];

	pc_begin("XBD.6.4-charmap-description.004");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "<x-first> \\xe1\n<x-second> \\xe1\n<x-cap> \\xc1\n", "", 0);
	ld_ok("twon", "cmap",
	    "LC_CTYPE\nlower <x-first>\nupper <x-cap>\ntoupper (<x-second>,<x-cap>)\ntolower (<x-cap>,<x-first>)\nEND LC_CTYPE\n" LD_CP_COLLATE
	    LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	pc_check(islower(0xe1) != 0, "byte 0xe1, listed under its first name, is not lower-case");
	pc_check(isupper(0xc1) != 0, "byte 0xc1 is not upper-case");
	pc_check(toupper(0xe1) == 0xc1, "toupper of 0xe1, mapped under its second name, is %#x, not 0xc1", (unsigned)toupper(0xe1));
	pc_check(tolower(0xc1) == 0xe1, "tolower of 0xc1, mapped to the first name, is %#x, not 0xe1", (unsigned)tolower(0xc1));
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
