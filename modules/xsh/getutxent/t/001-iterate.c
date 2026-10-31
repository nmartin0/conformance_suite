/*
 * Reading the user accounting database from the start ends with a null pointer.
 * (An entry of a type the header does not list is not judged: the page does
 * not forbid further types.)
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getutxent.001
 */
#include "utx.h"

int main(void)
{
	struct utmpx e[UTX_MAX];
	long n, kept;

	pc_begin("XSH.getutxent.001");
	pc_test();
	n = utx_read_all(e, &kept);
	endutxent();
	if (n == 0)
		pc_notinuse("the user accounting database is absent, empty or hidden");
	(void)kept;
	pc_pass();
}
