/*
 * An orphan gets a new parent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.020
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
	int p[2];
	pid_t mid;
	struct { pid_t mid, after; } rep;
	int st;

	pc_begin("XSH.wait.020");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	mid = fork();
	if (mid == -1)
		pc_unresolved("fork failed");
	if (mid == 0) {
		pid_t me = getpid(), g;	/* known before the grandchild exists */
		(void)close(p[0]);
		g = fork();
		if (g == -1)
			_exit(9);
		if (g == 0) {
			struct timespec ts = { 0, 100000000 };
			int i;
			for (i = 0; i < 100 && getppid() == me; i++)
				(void)nanosleep(&ts, NULL);	/* until the middle process is gone */
			rep.mid = me;
			rep.after = getppid();
			(void)write(p[1], &rep, sizeof rep);
			_exit(0);
		}
		_exit(0);	/* leaves g behind */
	}
	(void)close(p[1]);	/* only the grandchild holds the write end now */
	pc_check(waitpid(mid, &st, 0) == mid, "waitpid failed");
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
		pc_unresolved("the middle process could not fork");
	pc_check(read(p[0], &rep, sizeof rep) == (ssize_t)sizeof rep, "the grandchild did not report");
	pc_check(rep.mid == mid, "the grandchild's first parent was %ld, not %ld", (long)rep.mid, (long)mid);
	pc_check(rep.after != mid, "the orphan still has the dead process %ld as parent", (long)mid);
	pc_pass();
}
