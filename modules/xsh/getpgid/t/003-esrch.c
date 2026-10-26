/*
 * getpgid() of a process that no longer exists fails with ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpgid.003
 * MUTANT    getpgid.esrch_einval
 * MUTANT    getpgid.esrch_self
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* The ID of a reaped child, used by no process and no process group now. */
static pid_t gone_pid(void)
{
	pid_t pid = fork();
	int st;

	if (pid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (pid == 0)
		_exit(0);
	while (waitpid(pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (kill(pid, 0) == 0 || errno != ESRCH || kill(-pid, 0) == 0 || errno != ESRCH)
		pc_unresolved("process ID %ld is in use again", (long)pid);
	return pid;
}

static int still_gone(pid_t pid)
{
	return kill(pid, 0) == -1 && errno == ESRCH && kill(-pid, 0) == -1 && errno == ESRCH;
}

int main(void)
{
	pid_t pid, r;

	pc_begin("XSH.getpgid.003");
	pid = gone_pid();
	pc_test();
	r = getpgid(pid);
	if (r != -1 && !still_gone(pid))
		pc_unresolved("process ID %ld was reused during the test", (long)pid);
	pc_expect_err((long)r, ESRCH, "getpgid(<reaped child>)");
	pc_pass();
}
