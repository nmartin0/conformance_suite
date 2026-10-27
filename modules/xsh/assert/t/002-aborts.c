/*
 * A failed assert() calls abort().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.assert.002
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

static void on_abrt(int sig)
{
	(void)sig;
	_exit(42);
}

static int run(int catch)
{
	pid_t c;
	int st;

	c = fork();
	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		int fd = open("/dev/null", O_WRONLY);

		if (fd >= 0)
			(void)dup2(fd, 2);
		if (catch) {
			struct sigaction sa;

			memset(&sa, 0, sizeof sa);
			sa.sa_handler = on_abrt;
			sigemptyset(&sa.sa_mask);
			(void)sigaction(SIGABRT, &sa, NULL);
		}
		alarm(20);
		assert(1 == 2);
		_exit(0);	/* reached only if assert() returned */
	}
	while (waitpid(c, &st, 0) == -1)
		;
	return st;
}

int main(void)
{
	struct sigaction sa;
	int st;

	pc_begin("XSH.assert.002");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(60);
	pc_test();
	st = run(0);
	pc_check(WIFSIGNALED(st) && WTERMSIG(st) == SIGABRT, "the child status is %#x, expected death by SIGABRT", st);
	st = run(1);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 42, "with SIGABRT caught the child status is %#x, expected the handler's exit 42", st);
	pc_pass();
}
