/*
 * The stack pointer at process entry is a multiple of 16 (derived from argv).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.002
 */
#include "../../../../check.h"
#include <stdint.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.002")
#else
int main(int argc, char **argv)
{
	uintptr_t sp;

	pc_begin("ABI.i386.entry.002");
	pc_test();
	(void)argc;
	/* Follows the current revision (16 bytes); the older one says 4. Inferred as in entry.001. */
	sp = (uintptr_t)argv - 4;
	pc_check((sp & 15u) == 0, "initial ESP (argv - 4) is 0x%08lx, not a multiple of 16", (unsigned long)sp);
	pc_pass();
}
#endif
