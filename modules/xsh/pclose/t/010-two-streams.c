/*
 * Each stream's pclose() gives its own command's status.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pclose.010
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
	FILE *a, *b;
	int ra, rb;

	pc_begin("XSH.pclose.010");
	guard(20);
	a = start("exit 3", "r");
	b = start("exit 4", "r");
	pc_test();
	rb = pclose(b);
	ra = pclose(a);
	pc_check(rb != -1 && WIFEXITED(rb) && WEXITSTATUS(rb) == 4, "pclose() of the second stream gave %#x, expected exit 4", rb);
	pc_check(ra != -1 && WIFEXITED(ra) && WEXITSTATUS(ra) == 3, "pclose() of the first stream gave %#x, expected exit 3", ra);
	pc_pass();
}
