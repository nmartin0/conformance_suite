/*
 * A character after the escape character in a symbolic name stands for
 * itself: a charmap name written with an escaped greater-than sign
 * defines a name containing that sign, which the locale source can use
 * (written the same way) to put the character in a class.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.4-charmap-description.008
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>

int main(void)
{
	char loc[4200];

	pc_begin("XBD.6.4-charmap-description.008");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "<x-a\\>b> \\xe1\n<x-c\\\\d> \\xe2\n", "", 0);
	ld_ok("escn", "cmap",
	    "LC_CTYPE\nlower <x-a\\>b>;<x-c\\\\d>\nEND LC_CTYPE\n" LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	pc_check(islower(0xe1) != 0, "byte 0xe1, named <x-a\\>b> with an escaped greater-than sign, is not lower-case");
	pc_check(islower(0xe2) != 0, "byte 0xe2, named <x-c\\\\d> with an escaped escape character, is not lower-case");
	pc_check(!islower(0xe3), "the byte 0xe3, which no name defines, is lower-case");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
