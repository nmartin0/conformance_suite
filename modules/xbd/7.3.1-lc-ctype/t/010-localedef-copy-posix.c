/*
 * A locale made with localedef whose categories all copy the POSIX locale
 * behaves as the POSIX locale: classes, case mappings, collation, langinfo
 * items and localeconv() answer alike for the portable characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.010
 */
#include "pc.h"
#include "ldef.h"
#include "pcbat.h"

int main(void)
{
	char loc[4200];
	size_t d;

	pc_begin("XBD.7.3.1-lc-ctype.010");
	ld_need();
	pcc_init();
	ld_ok("copyall", NULL, LD_CP_ALL, loc, sizeof loc);
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pcb_sig(pcb_a, sizeof pcb_a, 0);
	pc_test();
	ld_load(loc);
	pcb_sig(pcb_b, sizeof pcb_b, 0);
	d = pcb_diff(pcb_a, pcb_b);
	pc_check(strcmp(pcb_a, pcb_b) == 0, "the copy of the POSIX locale differs from it in %s (offset %lu)", pcb_part(pcb_a, d),
	    (unsigned long)d);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
