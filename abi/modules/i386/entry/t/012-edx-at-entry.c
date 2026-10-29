/*
 * EDX at the first instruction of the program is a function the application
 * is to register for exit; the test sees it through an entry point of its own.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.012
 */
#include "../../../../check.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.012")
#else
ABI_ENTRY_STATE_STUB();

int main(void)
{
	void (*fn)(void);
	pid_t p;
	int st;

	pc_begin("ABI.i386.entry.012");
	if (abi_ent_seen != ABI_ENT_MAGIC)
		pc_notinuse("the entry point of this test was not used (the link option was refused)");
	pc_test();
	/* The text does not say what zero means, so a zero EDX asserts nothing. */
	if (abi_ent_edx == 0)
		pc_notinuse("EDX was zero at entry: the text gives no meaning to that value, so there is nothing to check");
	/*
	 * The text asks the application to register the function to run at exit.
	 * Do exactly that in a child and let it exit normally: a value that is
	 * not the address of executable code ends the child by a signal.
	 */
	fn = (void (*)(void))(uintptr_t)abi_ent_edx;
	fflush(NULL);
	p = fork();
	if (p < 0)
		pc_unresolved("cannot create a process: fork failed");
	if (p == 0) {
		if (atexit(fn) != 0)
			_exit(3);
		exit(0);
	}
	if (waitpid(p, &st, 0) != p)
		pc_unresolved("waitpid failed");
	pc_check(!WIFSIGNALED(st), "registering EDX (0x%08x) and exiting ended the process with signal %d: not a callable function",
	    abi_ent_edx, WIFSIGNALED(st) ? WTERMSIG(st) : 0);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "registering EDX (0x%08x) and exiting gave status %d, not a normal exit",
	    abi_ent_edx, WIFEXITED(st) ? WEXITSTATUS(st) : -1);
	pc_pass();
}
#endif
