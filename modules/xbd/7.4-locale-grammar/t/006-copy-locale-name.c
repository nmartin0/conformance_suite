/*
 * The copy statement takes the locale name as a bare word or in double
 * quotes: both forms, naming the POSIX locale, make categories that behave
 * as the POSIX locale's.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.4-locale-grammar.006
 */
#include "pc.h"
#include "ldef.h"
#include "pcbat.h"

#define BARE(cat) cat "\ncopy POSIX\nEND " cat "\n"

int main(void)
{
	char lq[4200], lb[4200];
	size_t d;

	pc_begin("XBD.7.4-locale-grammar.006");
	ld_need();
	pcc_init();
	ld_ok("copyq", NULL, LD_CP_ALL, lq, sizeof lq);
	ld_ok("copyb", NULL, BARE("LC_CTYPE") BARE("LC_COLLATE") BARE("LC_MONETARY") BARE("LC_NUMERIC") BARE("LC_TIME") BARE("LC_MESSAGES"), lb,
	    sizeof lb);
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pcb_sig(pcb_a, sizeof pcb_a, 0);
	pc_test();
	ld_load(lq);
	pcb_sig(pcb_b, sizeof pcb_b, 0);
	d = pcb_diff(pcb_a, pcb_b);
	pc_check(strcmp(pcb_a, pcb_b) == 0, "the locale with quoted copy names differs from POSIX in %s", pcb_part(pcb_a, d));
	ld_load(lb);
	pcb_sig(pcb_b, sizeof pcb_b, 0);
	d = pcb_diff(pcb_a, pcb_b);
	pc_check(strcmp(pcb_a, pcb_b) == 0, "the locale with bare copy names differs from POSIX in %s", pcb_part(pcb_a, d));
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
