/*
 * The stack pointer at process entry is a multiple of 4 (derived from argv).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.001
 */
#include "../../../../check.h"
#include <stdint.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.001")
#else
int main(int argc, char **argv)
{
	uintptr_t sp;

	pc_begin("ABI.i386.entry.001");
	pc_test();
	(void)argc;
	/*
	 * Inferred, not observed: the initial stack holds argc at ESP and the
	 * argument pointers from ESP+4, and main() is handed a pointer to
	 * them, so the initial ESP is the address of argv[0] less 4.
	 */
	sp = (uintptr_t)argv - 4;
	pc_check((sp & 3u) == 0, "initial ESP (argv - 4) is 0x%08lx, not a multiple of 4", (unsigned long)sp);
	pc_pass();
}
#endif
