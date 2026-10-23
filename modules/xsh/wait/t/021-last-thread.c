/*
 * A child ending with its last thread reports 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.021
 * MUTANT    wait.status_garbage
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH.wait.021");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0)
		pthread_exit((void *)1);	/* the value must not become the status */
	pc_check(waitpid(c, &st, 0) == c, "waitpid failed");
	pc_check(st == 0, "status %#x, expected 0", st);
	pc_pass();
}
