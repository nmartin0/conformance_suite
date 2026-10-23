/*
 * fork() cancels the alarm in the child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.008
 * MUTANT    fork.child_alarm_kept
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Waits for the child; returns its exit status (0 = all its checks held).
 * A child that did not exit normally is UNRESOLVED.
 */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (!WIFEXITED(st))
		pc_unresolved("the child was killed by signal %d", WIFSIGNALED(st) ? WTERMSIG(st) : 0);
	return WEXITSTATUS(st);
}

int main(void)
{
	pid_t r;
	int code;

	pc_begin("XSH.fork.008");
	signal(SIGALRM, SIG_IGN);
	(void)alarm(100);
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0)
		_exit(alarm(0) == 0 ? 0 : 1);
	code = reap(r);
	pc_check(code == 0, "the child still had the parent's alarm");
	pc_pass();
}
