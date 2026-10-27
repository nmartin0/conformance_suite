/*
 * Reading from the start visits entries and ends with a null pointer that leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrent.001
 */
#include "grdb.h"

int main(void)
{
	xent_t *p;
	long n = 0;

	pc_begin("XSH.getgrent.001");
	pc_test();
	setgrent();
	for (;;) {
		errno = XSENTINEL;
		p = getgrent();
		if (p == NULL)
			break;
		if (++n > 1000000)
			pc_unresolved("more than a million entries");
		pc_check(XNAME(p) != NULL, "entry %ld has no name", n);
	}
	if (errno != XSENTINEL) {
		if (xc_mayfail(errno))
			pc_unresolved("the read reported %s", pc_errname(errno));
		pc_fail("errno changed to %s at the end of the database", pc_errname(errno));
	}
	if (n == 0)
		pc_unresolved("no entry is visible");
	endgrent();
	pc_pass();
}
