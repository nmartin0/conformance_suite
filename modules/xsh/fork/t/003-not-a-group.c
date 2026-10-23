/*
 * The child's ID is not a process group ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.003
 * MUTANT    fork.pgid_is_pid
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
	int p[2], q[2];
	pid_t r;
	char c;

	pc_begin("XSH.fork.003");
	pc_check(pipe(p) == 0 && pipe(q) == 0, "pipe failed");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		(void)write(p[1], "r", 1);	/* ready */
		(void)read(q[0], &c, 1);	/* wait until the parent has looked */
		_exit(0);
	}
	pc_check(read(p[0], &c, 1) == 1, "the child did not start");
	errno = 0;
	pc_check(kill(-r, 0) == -1 && errno == ESRCH,
	    "a process group with the child's ID %ld exists", (long)r);
	(void)write(q[1], "g", 1);
	(void)reap(r);
	pc_pass();
}
