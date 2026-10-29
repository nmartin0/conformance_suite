/*
 * The direction flag is clear when main() is entered.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.006
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.006")
#else
int main(void)
{
	unsigned f;

	f = abi_eflags();       /* first, before the test helper runs */
	pc_begin("ABI.i386.entry.006");
	pc_test();
	pc_check((f & ABI_EFLAGS_DF) == 0, "EFLAGS is 0x%08x in main(): the direction flag is set", f);
	pc_pass();
}
#endif
