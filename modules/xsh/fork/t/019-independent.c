/*
 * Parent and child run at the same time, each answering the other.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.019
 * MUTANT    fork.child_waits_parent
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	int up[2], down[2], i, st;
	pid_t r;
	char c;

	pc_begin("XSH.fork.019");
	pc_check(pipe(up) == 0 && pipe(down) == 0, "pipe failed");
	pc_test();
	alarm(10);	/* a deadlock ends the test with SIGALRM: FAIL */
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		for (i = 0; i < 100; i++)
			if (read(down[0], &c, 1) != 1 || write(up[1], &c, 1) != 1)
				_exit(1);
		_exit(0);
	}
	for (i = 0; i < 100; i++) {
		pc_check(write(down[1], "p", 1) == 1, "write to the child failed");
		pc_check(read(up[0], &c, 1) == 1, "the child did not answer");
	}
	pc_check(waitpid(r, &st, 0) == r && WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed");
	pc_pass();
}
