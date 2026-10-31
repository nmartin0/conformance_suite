/*
 * Fixture for ci/mutate.sh: pcfz_pid() reaches getppid() through a header
 * that redirects the name, so a fault is seen only if the fault library
 * defines the symbol the header names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pcfz.001
 * MUTANT    pcfz.zero
 */
#include "pc.h"
#include "../pcfz.h"

int main(void)
{
	pc_begin("XSH.pcfz.001");
	if (pcfz_pid() <= 0)
		pc_unresolved("pcfz_pid() failed in setup");
	pc_test();
	pc_check(pcfz_pid() > 0, "pcfz_pid() returned %ld", (long)pcfz_pid());
	pc_pass();
}
