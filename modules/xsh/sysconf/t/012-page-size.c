/*
 * sysconf(_SC_PAGE_SIZE) equals sysconf(_SC_PAGESIZE).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sysconf.012
 * MUTANT    sysconf.page_size_zero
 */
#include "pc.h"
#include <errno.h>
#include <unistd.h>

int main(void)
{
	long a, b;

	pc_begin("XSH.sysconf.012");
	pc_test();
	a = sysconf(_SC_PAGESIZE);
	b = sysconf(_SC_PAGE_SIZE);
	pc_check(a >= 1, "sysconf(_SC_PAGESIZE) returned %ld", a);
	pc_check(a == b, "sysconf(_SC_PAGE_SIZE) returned %ld, sysconf(_SC_PAGESIZE) %ld", b, a);
	pc_pass();
}
