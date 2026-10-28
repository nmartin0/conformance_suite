/*
 * A range line <r01>...<r03> \xe5 defines the three names <r01>, <r02>
 * and <r03> with consecutive encodings starting at the stated byte:
 * characters at those three bytes are lower-case when the three names are
 * listed, and the byte after them is not; a range of two-digit numbers
 * running through 09 to 10 names four consecutive bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.4-charmap-description.003
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>

int main(void)
{
	char loc[4200];
	int i;

	pc_begin("XBD.6.4-charmap-description.003");
	ld_need();
	pcc_init();
	for (i = 0; i < PCC_N; i++)
		if ((unsigned char)pcc_all[i] >= 0xc4 && (unsigned char)pcc_all[i] <= 0xe9)
			pc_notinuse("a portable character is encoded in the high range the test uses");
	ld_charmap("cmap", LD_HDR, '\\', "<r01>...<r03> \\xe5\n<u08>...<u11> \\xc5\n", "", 0);
	ld_ok("range", "cmap",
	    "LC_CTYPE\nlower <r01>;<r02>;<r03>\nupper <u08>;<u09>;<u10>;<u11>\nEND LC_CTYPE\n" LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC
	    LD_CP_TIME LD_CP_MESSAGES,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	pc_check(islower(0xe5) && islower(0xe6) && islower(0xe7), "the three names of <r01>...<r03> \\xe5 are not at 0xe5, 0xe6, 0xe7");
	pc_check(!islower(0xe4) && !islower(0xe8), "a byte outside the range is lower-case");
	pc_check(isupper(0xc5) && isupper(0xc6) && isupper(0xc7) && isupper(0xc8), "the names <u08>...<u11> \\xc5 are not at four consecutive bytes from 0xc5 across the carry from 09 to 10");
	pc_check(!isupper(0xc4) && !isupper(0xc9), "a byte outside the second range is upper-case");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
