/*
 * pclose() fails with ECHILD when the caller took the status.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pclose.008
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

/* popen() is setup here: the call under test is pclose(). */
static FILE *start(const char *cmd, const char *mode)
{
	FILE *f = popen(cmd, mode);

	if (f == NULL)
		pc_unresolved("popen(\"%s\", \"%s\") failed: %s", cmd, mode, pc_errname(errno));
	return f;
}

int main(void)
{
	FILE *f;
	pid_t w;
	int r, st;

	pc_begin("XSH.pclose.008");
	guard(20);
	f = start("exit 5", "r");
	w = waitpid(-1, &st, 0);	/* the only child is the command's shell */
	if (w == -1)
		pc_unresolved("waitpid(-1) failed: %s", pc_errname(errno));
	pc_test();
	r = pclose(f);
	pc_check(r == -1, "pclose() returned %d after the status was taken by waitpid()", r);
	pc_check(errno == ECHILD, "pclose() failed with %s, expected ECHILD", pc_errname(errno));
	pc_pass();
}
