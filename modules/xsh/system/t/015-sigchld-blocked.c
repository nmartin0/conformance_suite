/*
 * SIGCHLD is blocked while system() waits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.015
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* A call that blocks must fail the test, not hang it. */
static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: the call under test did not finish in time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

static void on_chld(int sig)
{
	int saved = errno;

	(void)sig;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
	errno = saved;
}

int main(void)
{
	struct sigaction sa;
	int r;

	pc_begin("XSH.system.015");
	guard(30);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_chld;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGCHLD, &sa, NULL) == 0, "sigaction failed");
	pc_test();
	r = system("sleep 1; exit 3");
	pc_check(r != -1, "system() failed (%s): the SIGCHLD handler took the status", pc_errname(errno));
	pc_check(WIFEXITED(r) && WEXITSTATUS(r) == 3, "status %#x, expected exit 3", r);
	pc_pass();
}
