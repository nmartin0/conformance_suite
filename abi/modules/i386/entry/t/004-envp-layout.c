/*
 * The environment pointers follow the argument pointers' terminator and end with a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.004
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.004")
#else
int main(int argc, char **argv, char **envp)
{
	long n = 0;

	pc_begin("ABI.i386.entry.004");
	pc_test();
	/* Inferred: main() is handed pointers into the initial stack block. */
	pc_check(envp == argv + argc + 1,
	    "envp is %p, expected the word after argv[argc] (%p)", (void *)envp, (void *)(argv + argc + 1));
	while (envp[n] != NULL) {
		pc_check(n < 1000000, "no null pointer ends the environment array");
		n++;
	}
	pc_pass();
}
#endif
