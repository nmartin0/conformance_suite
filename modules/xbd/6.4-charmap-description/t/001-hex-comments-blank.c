/*
 * A charmap with comment lines, empty lines, a leading comment and
 * trailing comments on mapping lines is accepted; the hexadecimal
 * encodings it gives to additional characters are the bytes the created
 * locale classifies: lower-case, upper-case and blank characters at bytes
 * the charmap names, none of them at the neighbouring bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.4-charmap-description.001
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>

int main(void)
{
	char loc[4200];

	pc_begin("XBD.6.4-charmap-description.001");
	ld_need();
	pcc_init();
	ld_charmap("cmap", "# a leading comment\n\n" LD_HDR "\n# another comment\n", '\\',
	    "\n# comment between mappings\n\n<x-low1> \\xe1 trailing comment\n\n<x-upp1> \\xc1\n# comment\n<x-blk1> \\xa0 comment\n\n", "\n", 1);
	ld_ok("hexc", "cmap",
	    "LC_CTYPE\nlower <x-low1>\nupper <x-upp1>\nblank <x-blk1>\nEND LC_CTYPE\n" LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC
	    LD_CP_TIME LD_CP_MESSAGES,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	pc_check(islower(0xe1) != 0, "byte 0xe1, named <x-low1> in hexadecimal, is not lower-case");
	pc_check(isupper(0xc1) != 0, "byte 0xc1, named <x-upp1> in hexadecimal, is not upper-case");
	pc_check(isblank(0xa0) != 0, "byte 0xa0, named <x-blk1> in hexadecimal, is not a blank");
	pc_check(!islower(0xe0) && !islower(0xe2) && !isupper(0xc0) && !isupper(0xc2) && !isblank(0x9f) && !isblank(0xa1),
	    "a byte next to the listed ones is in a class the source does not give it");
	pc_check(islower('a') && isupper('A') && isdigit('0') && isspace(' '), "a portable character is not in its class");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
