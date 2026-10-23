/*
 * FD_CLOFORK keeps a descriptor out of the child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.005
 * MUTANT    fork.child_keeps_clofork
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
	int a, b;
	pid_t r;
	int code;

	pc_begin("XSH.fork.005");
	pc_make_file("f", "x");
	a = open("f", O_RDONLY);
	b = open("f", O_RDONLY);
	pc_check(a >= 0 && b >= 0, "cannot open f");
#ifndef FD_CLOFORK
	pc_unresolved("<fcntl.h> does not define FD_CLOFORK");
#else
	pc_check(fcntl(a, F_SETFD, FD_CLOFORK) == 0, "cannot set FD_CLOFORK");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		if (fcntl(a, F_GETFD) != -1)
			_exit(1);
		if (fcntl(b, F_GETFD) == -1)
			_exit(2);
		_exit(0);
	}
	code = reap(r);
	pc_check(code != 1, "the FD_CLOFORK descriptor is open in the child");
	pc_check(code != 2, "a descriptor without FD_CLOFORK is missing in the child");
	pc_check(code == 0, "child status %d", code);
	pc_pass();
#endif
}
