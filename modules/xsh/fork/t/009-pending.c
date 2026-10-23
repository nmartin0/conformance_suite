/*
 * The child has no pending signals.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.009
 * MUTANT    fork.child_pending_kept
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
	sigset_t s;
	pid_t r;
	int code;

	pc_begin("XSH.fork.009");
	sigemptyset(&s);
	sigaddset(&s, SIGUSR1);
	pc_check(sigprocmask(SIG_BLOCK, &s, NULL) == 0, "sigprocmask failed");
	pc_check(raise(SIGUSR1) == 0, "raise failed");
	pc_check(sigpending(&s) == 0 && sigismember(&s, SIGUSR1) == 1, "SIGUSR1 is not pending");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		sigset_t c;
		if (sigpending(&c) != 0)
			_exit(9);
		_exit(sigismember(&c, SIGUSR1) == 1 ? 1 : 0);
	}
	code = reap(r);
	pc_check(code == 0, "SIGUSR1 is pending in the child");
	pc_pass();
}
