/*
 * waitpid() of a non-child gives ECHILD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.016
 * MUTANT    wait.echild_as_einval
 * MUTANT    wait.pid_ignored
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	int st;
	long r;
	pid_t c;

	pc_begin("XSH.wait.016");
	/*
	 * A live child, so that "no unwaited-for children" does not apply:
	 * the only error left is that the pid is not a child.
	 */
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		struct timespec ts = { 20, 0 };
		(void)nanosleep(&ts, NULL);
		_exit(0);
	}
	pc_test();
	r = waitpid(getppid(), &st, WNOHANG);
	{
		int e = errno;
		(void)kill(c, SIGKILL);
		(void)waitpid(c, NULL, 0);
		errno = e;
	}
	pc_expect_err(r, ECHILD, "waitpid(<the parent>, WNOHANG) with a live child");
	pc_pass();
}
