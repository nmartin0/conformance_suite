/*
 * exit() status: the full value through waitid().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.002
 * MUTANT    exit.status_wrong
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Waits for pid and returns its raw status; anything odd is UNRESOLVED. */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	return st;
}

static pid_t spawn(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	return r;
}

int main(void)
{
	siginfo_t si;
	pid_t c;

	pc_begin("XSH.exit.002");
	pc_test();
	c = spawn();
	if (c == 0)
		exit(0x1234);
	memset(&si, 0, sizeof si);
	if (waitid(P_PID, (id_t)c, &si, WEXITED) != 0)
		pc_unresolved("waitid failed: %s", pc_errname(errno));
	pc_check(si.si_pid == c, "si_pid %ld, expected %ld", (long)si.si_pid, (long)c);
	pc_check(si.si_code == CLD_EXITED, "si_code %d, expected CLD_EXITED", si.si_code);
	pc_check(si.si_status == 0x1234, "si_status %#x, expected 0x1234", (unsigned)si.si_status);
	pc_pass();
}
