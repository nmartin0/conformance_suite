/*
 * assert() accepts any scalar type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.assert.005
 */
#include "pc.h"
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void on_alarm(int sig)
{
	(void)sig;
	pc_fail("a child did not finish in time");
}

/* Returns 1 if the assertion of kind k aborts, 0 if it passes. */
static int aborts(int k)
{
	pid_t c = fork();
	int st;

	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		int fd = open("/dev/null", O_WRONLY);
		int x = 1;
		int *null = NULL, *nonnull = &x;
		double zero = 0.0, half = 0.5;
		long lz = 0L, lm = -1L;

		if (fd >= 0)
			(void)dup2(fd, 2);
		alarm(20);
		switch (k) {
		case 0: assert(null); break;
		case 1: assert(zero); break;
		case 2: assert(lz); break;
		case 3: assert(nonnull); break;
		case 4: assert(half); break;
		default: assert(lm); break;
		}
		_exit(0);
	}
	while (waitpid(c, &st, 0) == -1)
		;
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGABRT)
		return 1;
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "unexpected child status %#x for kind %d", st, k);
	return 0;
}

int main(void)
{
	struct sigaction sa;
	int k;

	pc_begin("XSH.assert.005");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(60);
	pc_test();
	for (k = 0; k < 6; k++) {
		int a = aborts(k);

		if (k < 3)
			pc_check(a, "assert() of a false scalar (kind %d) did not abort", k);
		else
			pc_check(!a, "assert() of a true scalar (kind %d) aborted", k);
	}
	pc_pass();
}
