/*
 * ESP at the first instruction is 16-byte aligned and addresses the argument
 * count, followed by the argument pointers; seen through an entry point of the
 * test's own.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.014
 */
#include "../../../../check.h"
#include <stdint.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.014")
#else
ABI_ENTRY_STATE_STUB();

int main(int argc, char **argv)
{
	const unsigned *sp;

	pc_begin("ABI.i386.entry.014");
	if (abi_ent_seen != ABI_ENT_MAGIC)
		pc_notinuse("the entry point of this test was not used (the link option was refused)");
	pc_test();
	sp = (const unsigned *)(uintptr_t)abi_ent_esp;
	pc_check((abi_ent_esp & 15u) == 0, "ESP at the first instruction is 0x%08x, not a multiple of 16", abi_ent_esp);
	pc_check((int)sp[0] == argc, "the word at ESP is %d, the argument count is %d", (int)sp[0], argc);
	pc_check((uintptr_t)(sp + 1) == (uintptr_t)argv,
	    "the argument pointers start at 0x%08lx, main() was given 0x%08lx", (unsigned long)(sp + 1), (unsigned long)argv);
	pc_pass();
}
#endif
