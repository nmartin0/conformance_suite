/*
 * FD_CLOFORK descriptors of the caller are closed in the command.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.018
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
static void finish(FILE *f)
{
	if (pclose(f) == -1)
		pc_unresolved("pclose() failed: %s", pc_errname(errno));
}

#ifdef FD_CLOFORK
static int status_of(int fd)
{
	char cmd[128], b[32];
	FILE *f;

	(void)snprintf(cmd, sizeof cmd, "true 2>/dev/null <&%d; echo $?", fd);
	errno = 0;
	f = popen(cmd, "r");
	pc_check(f != NULL, "popen() failed: %s", pc_errname(errno));
	pc_check(fgets(b, sizeof b, f) != NULL, "the command printed nothing");
	finish(f);
	return atoi(b);
}
#endif

int main(void)
{
	pc_begin("XSH.popen.018");
#ifndef FD_CLOFORK
	pc_fail("<fcntl.h> does not define FD_CLOFORK");
#else
	int a, b;

	guard(20);
	pc_make_file("f", "x");
	a = open("f", O_RDONLY);
	b = open("f", O_RDONLY);
	pc_check(a >= 0 && b >= 0, "cannot open f");
	pc_check(fcntl(a, F_SETFD, FD_CLOFORK) == 0, "cannot set FD_CLOFORK");
	pc_test();
	pc_check(status_of(b) == 0, "the descriptor without FD_CLOFORK was not open in the command");
	pc_check(status_of(a) != 0, "the descriptor with FD_CLOFORK was still open in the command");
	pc_pass();
#endif
}
