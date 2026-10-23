/*
 * The child has its own ID and the parent as its parent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.002
 * MUTANT    fork.child_gets_pid
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
	pid_t parent = getpid(), r;
	int code;

	pc_begin("XSH.fork.002");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0 || (r > 0 && getpid() != parent)) {
		/* The child, also when fork() wrongly returned non-zero to it. */
		if (r != 0)
			_exit(3);
		if (getpid() == parent)
			_exit(1);
		if (getppid() != parent)
			_exit(2);
		_exit(0);
	}
	pc_check(r != parent, "the child's ID equals the parent's");
	code = reap(r);
	pc_check(code != 1, "the child's getpid() is the parent's ID");
	pc_check(code != 2, "the child's getppid() is not the parent's ID");
	pc_check(code != 3, "fork() returned %ld in the child", (long)r);
	pc_check(code == 0, "child status %d", code);
	pc_pass();
}
