/*
 * pclose() closes the stream before it waits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pclose.011
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
	struct sigaction sa;
	FILE *f;
	int r;

	pc_begin("XSH.pclose.011");
	guard(30);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = SIG_DFL;	/* the command inherits it */
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGPIPE, &sa, NULL) == 0, "sigaction failed");
	f = start("while :; do echo x || exit 9; done", "r");
	pc_check(fgetc(f) == 'x', "the command did not start writing");
	pc_test();
	r = pclose(f);
	pc_check(r != -1, "pclose() failed: %s", pc_errname(errno));
	pc_check(WIFEXITED(r) || WIFSIGNALED(r), "status %#x is neither an exit nor a signal status", r);
	pc_pass();
}
