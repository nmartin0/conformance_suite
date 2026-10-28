/*
 * An ellipsis between two characters of a class list puts every character
 * with an encoded value between them in the class, both for symbolic names
 * and for hexadecimal constants.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.009
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>

int main(void)
{
	char loc[4200];
	int b;

	pc_begin("XBD.7.3.1-lc-ctype.009");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\',
	    "<el-pre> \\xcf\n<el-a> \\xd0\n<el-b> \\xd1\n<el-c> \\xd2\n<el-out> \\xd3\n"
	    "<eu-pre> \\xdf\n<eu-a> \\xe0\n<eu-b> \\xe1\n<eu-c> \\xe2\n<eu-d> \\xe3\n<eu-e> \\xe4\n<eu-out> \\xe5\n",
	    "", 0);
	ld_ok("ell", "cmap",
	    "LC_CTYPE\nlower <el-a>;...;<el-c>\nupper \\xe0;...;\\xe4\nEND LC_CTYPE\n" LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC
	        LD_CP_TIME LD_CP_MESSAGES,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	for (b = 0xd0; b <= 0xd2; b++)
		pc_check(islower(b) != 0 && !isupper(b), "byte %#x (inside the symbolic ellipsis) is not in lower only", b);
	for (b = 0xe0; b <= 0xe4; b++)
		pc_check(isupper(b) != 0 && !islower(b), "byte %#x (inside the hexadecimal ellipsis) is not in upper only", b);
	pc_check(!islower(0xcf) && !isupper(0xcf) && !islower(0xd3) && !isupper(0xd3), "a byte just outside the symbolic ellipsis is in lower or upper");
	pc_check(!islower(0xdf) && !isupper(0xdf) && !islower(0xe5) && !isupper(0xe5), "a byte just outside the hexadecimal ellipsis is in lower or upper");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
