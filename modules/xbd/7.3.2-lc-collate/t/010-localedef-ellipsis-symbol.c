/*
 * An ellipsis in a collation order puts the characters between its
 * neighbours in the order of their encoded values; an initial ellipsis
 * stands for everything below the next character and a trailing ellipsis
 * for everything above the previous one; a collating symbol declared with
 * collating-symbol and placed in the order is accepted without effect on
 * how characters collate; an order_start without operands is a single
 * forward level.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.010
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char mid[4200], ini[4200], trl[4200];

	pc_begin("XBD.7.3.2-lc-collate.010");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("ell", "cmap",
	    "LC_COLLATE\ncollating-symbol <HIGH>\norder_start\n<z>\n<a>\n...\n<e>\n<HIGH>\n<y>\nUNDEFINED\norder_end\nEND LC_COLLATE\n" REST,
	    mid, sizeof mid);
	ld_ok("ellini", "cmap", "LC_COLLATE\norder_start\n...\n<e>\nUNDEFINED\norder_end\nEND LC_COLLATE\n" REST, ini, sizeof ini);
	ld_ok("elltrl", "cmap", "LC_COLLATE\norder_start\n<c>\n...\norder_end\nEND LC_COLLATE\n" REST, trl, sizeof trl);
	pc_test();
	ld_load(mid);
	pc_check(strcoll("z", "a") < 0, "z does not collate before a");
	pc_check(strcoll("a", "b") < 0 && strcoll("b", "c") < 0 && strcoll("c", "d") < 0 && strcoll("d", "e") < 0,
	    "the characters a to e do not collate in the order of their encoded values");
	pc_check(strcoll("e", "y") < 0, "e does not collate before y, which follows the collating symbol");
	pc_check(strcoll("y", "f") < 0, "y does not collate before the unlisted f");
	pc_check(strcoll("y", "z") > 0, "y collates before z");
	ld_load(ini);
	pc_check(strcoll("a", "b") < 0 && strcoll("b", "d") < 0 && strcoll("d", "e") < 0,
	    "with an initial ellipsis the characters below e do not collate in the order of their encoded values before e");
	pc_check(strcoll("e", "f") < 0 && strcoll("f", "g") < 0, "the characters after e do not follow it in ascending order");
	ld_load(trl);
	pc_check(strcoll("c", "d") < 0 && strcoll("d", "e") < 0 && strcoll("e", "z") < 0,
	    "with a trailing ellipsis the characters above c do not collate in the order of their encoded values after c");
	pc_check(strcoll("z", "a") < 0 && strcoll("z", "b") < 0,
	    "with a trailing ellipsis the characters below c, which are left unlisted, do not collate after the ones above c");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
