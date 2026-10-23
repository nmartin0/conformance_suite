/*
 * fork() returns 0 to the child and its ID to the parent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.001
 * MUTANT    fork.child_gets_pid
 * MUTANT    fork.parent_gets_self
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
	int p[2];
	pid_t r, seen = 0, parent = getpid();

	pc_begin("XSH.fork.001");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed (%s)", pc_errname(errno));
	if (getpid() != parent) {
		/*
		 * The child (told apart by its ID, not by fork()'s value) says
		 * what fork() returned to it, and its own ID.
		 */
		pid_t me[2];
		me[0] = r;
		me[1] = getpid();
		_exit(write(p[1], me, sizeof me) == (ssize_t)sizeof me ? 0 : 99);
	}
	(void)close(p[1]);
	pc_check(r > 0, "fork() returned %ld in the parent", (long)r);
	{
		pid_t me[2];
		pc_check(read(p[0], me, sizeof me) == (ssize_t)sizeof me, "the child did not report");
		pc_check(me[0] == 0, "fork() returned %ld in the child, expected 0", (long)me[0]);
		seen = me[1];
	}
	pc_check(seen == r, "the parent got %ld, the child's ID is %ld", (long)r, (long)seen);
	pc_check(reap(r) == 0, "the child failed");
	pc_pass();
}
