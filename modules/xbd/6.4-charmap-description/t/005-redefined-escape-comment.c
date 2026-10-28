/*
 * A charmap that redefines its escape character and comment character
 * in the declarations before the mapping lines is read in the new
 * notation: encodings written with the new escape character denote the
 * same bytes, a line starting with the new comment character is ignored,
 * and the number sign is no longer special.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.4-charmap-description.005
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>

int main(void)
{
	char loc[4200];

	pc_begin("XBD.6.4-charmap-description.005");
	ld_need();
	pcc_init();
	ld_charmap("cmap", "<escape_char> /\n<comment_char> %\n" LD_HDR, '/',
	    "% a comment line with <x-bogus> /xe9 in it\n<x-low1> /xe1\n<x-upp1> /d193\n<x-blk1> /240\n", "", 0);
	ld_ok("escc", "cmap",
	    "LC_CTYPE\nlower <x-low1>\nupper <x-upp1>\nblank <x-blk1>\nEND LC_CTYPE\n" LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC
	    LD_CP_TIME LD_CP_MESSAGES,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	pc_check(islower(0xe1) != 0, "byte 0xe1, written /xe1 with escape character '/', is not lower-case");
	pc_check(isupper(0xc1) != 0, "byte 0xc1, written /d193, is not upper-case");
	pc_check(isblank(0xa0) != 0, "byte 0xa0, written /240, is not a blank");
	pc_check(!islower(0xe9) && !isalpha(0xe9), "the byte named on a comment line (0xe9) is lower-case or alphabetic");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
