/*
 * The C locale and the POSIX locale are identical: the answers for the
 * portable characters (classes, case mappings, collation), the langinfo
 * items, localeconv() and the code set are the same with "C" and with
 * "POSIX".
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.2-posix-locale.001
 */
#include "pc.h"
#include "pcbat.h"

int main(void)
{
	size_t d;

	pc_begin("XBD.7.2-posix-locale.001");
	pcc_init();
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pcb_sig(pcb_a, sizeof pcb_a, 1);
	pc_check(pcb_a[0] != '\0', "no description could be made");
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pcb_sig(pcb_b, sizeof pcb_b, 1);
	pc_test();
	d = pcb_diff(pcb_a, pcb_b);
	pc_check(strcmp(pcb_a, pcb_b) == 0, "\"C\" and \"POSIX\" differ in %s (at offset %lu)", pcb_part(pcb_a, d),
	    (unsigned long)d);
	pc_pass();
}
