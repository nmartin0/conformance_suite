/*
 * Reading the user accounting database from the start ends, and every entry has a defined type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getutxent.001
 */
#include "utx.h"

int main(void)
{
	struct utmpx e[UTX_MAX];
	long n, kept, i;

	pc_begin("XSH.getutxent.001");
	pc_test();
	n = utx_read_all(e, &kept);
	endutxent();
	if (n == 0)
		pc_notinuse("the user accounting database is absent, empty or hidden");
	for (i = 0; i < kept; i++)
		if (!utx_known_type(e[i].ut_type))
			pc_unresolved("entry %ld has a type (%d) outside the eight the header defines", i, (int)e[i].ut_type);
	pc_pass();
}
