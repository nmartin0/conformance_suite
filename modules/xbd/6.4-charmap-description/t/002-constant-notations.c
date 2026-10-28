/*
 * Hexadecimal (upper- and lower-case digits), three-digit decimal,
 * three-digit octal, two-digit decimal and two-digit octal constants in
 * a charmap each denote the byte they state: characters defined with the
 * six notations are the lower-case characters at those bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.4-charmap-description.002
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>

int main(void)
{
	char loc[4200], extra[600];
	int lo[2], nlo = 0, b, i, bytes[6];

	pc_begin("XBD.6.4-charmap-description.002");
	ld_need();
	pcc_init();
	for (b = 8; b < 64 && nlo < 2; b++)
		if ((pcc_flags[b] & PCC_PORT) == 0)
			lo[nlo++] = b;
	for (i = 0; i < PCC_N; i++)
		if ((unsigned char)pcc_all[i] >= 0xe1 && (unsigned char)pcc_all[i] <= 0xe4)
			pc_notinuse("a portable character is encoded in the high range the test uses");
	if (nlo < 2)
		pc_unresolved("no two unused low byte values found");
	bytes[0] = 0xe1;
	bytes[1] = 0xe2;
	bytes[2] = 0xe3;
	bytes[3] = 0xe4;
	bytes[4] = lo[0];
	bytes[5] = lo[1];
	(void)snprintf(extra, sizeof extra,
	    "<n-hexu> \\xE1\n<n-hexl> \\xe2\n<n-dec3> \\d%03u\n<n-oct3> \\%03o\n<n-dec2> \\d%02u\n<n-oct2> \\%02o\n", 0xe3u, 0xe4u,
	    (unsigned)lo[0], (unsigned)lo[1]);
	ld_charmap("cmap", LD_HDR, '\\', extra, "", 0);
	ld_ok("nota", "cmap",
	    "LC_CTYPE\nlower <n-hexu>;<n-hexl>;<n-dec3>;<n-oct3>;<n-dec2>;<n-oct2>\nEND LC_CTYPE\n" LD_CP_COLLATE LD_CP_MONETARY
	    LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	for (i = 0; i < 6; i++)
		pc_check(islower(bytes[i]) != 0, "byte %#x, defined in notation number %d, is not lower-case", bytes[i], i + 1);
	pc_check(!islower(0xe5) && !islower(0xe0), "a byte next to the defined ones is lower-case");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
