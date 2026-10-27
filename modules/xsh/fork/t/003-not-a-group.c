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
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Waits (for at most about 10 s) for the child and returns its exit status.
 * A child that does not end, or that does not exit normally, is killed and
 * reaped and the test is UNRESOLVED.
 */
static int reap(pid_t pid)
{
	int st, i;
	pid_t w = 0;

	for (i = 0; i < 1000 && w == 0; i++) {
		w = waitpid(pid, &st, WNOHANG);
		if (w == 0) {
			struct timespec ts = { 0, 10000000L };
			(void)nanosleep(&ts, NULL);
		}
	}
	if (w == 0) {
		(void)kill(pid, SIGKILL);
		(void)waitpid(pid, &st, 0);
		pc_unresolved("the child did not end");
	}
	if (w != pid)
		pc_unresolved("waitpid failed");
	if (!WIFEXITED(st))
		pc_unresolved("the child was killed by signal %d", WIFSIGNALED(st) ? WTERMSIG(st) : 0);
	return WEXITSTATUS(st);
}

int main(void)
{
	int p[2], q[2], started, gone;
	pid_t r;
	char c;

	pc_begin("XSH.fork.003");
	pc_check(pipe(p) == 0 && pipe(q) == 0, "pipe failed");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		(void)close(p[0]);
		(void)close(q[1]);	/* so that the parent's exit ends the wait */
		(void)alarm(30);	/* and so does this, whatever happens */
		(void)write(p[1], "r", 1);	/* ready */
		(void)read(q[0], &c, 1);	/* wait until the parent has looked */
		_exit(0);
	}
	(void)close(p[1]);
	(void)close(q[0]);
	started = read(p[0], &c, 1) == 1;
	errno = 0;
	gone = kill(-r, 0) == -1 && errno == ESRCH;
	/* The child is let go and reaped before anything is judged. */
	(void)write(q[1], "g", 1);
	(void)close(q[1]);
	(void)reap(r);
	pc_check(started, "the child did not start");
	pc_check(gone, "a process group with the child's ID %ld exists", (long)r);
	pc_pass();
}
