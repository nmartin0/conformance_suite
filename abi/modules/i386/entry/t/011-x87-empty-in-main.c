/*
 * No value is on the x87 register stack when main() is entered.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.011
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.011")
#else
int main(void)
{
	unsigned n;

	n = abi_x87_used();     /* first, before the test helper runs */
	pc_begin("ABI.i386.entry.011");
	pc_test();
	pc_check(n == 0, "%u x87 register(s) are in use at the entry of main()", n);
	pc_pass();
}
#endif
