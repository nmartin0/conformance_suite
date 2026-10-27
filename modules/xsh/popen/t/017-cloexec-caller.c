/*
 * FD_CLOEXEC descriptors of the caller are closed in the command.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.017
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

/* The status of "true" with its input taken from descriptor fd, as the command reports it. */
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

int main(void)
{
	int a, b;

	pc_begin("XSH.popen.017");
	guard(20);
	pc_make_file("f", "x");
	a = open("f", O_RDONLY | O_CLOEXEC);
	b = open("f", O_RDONLY);
	pc_check(a >= 0 && b >= 0, "cannot open f");
	pc_test();
	pc_check(status_of(b) == 0, "the descriptor without FD_CLOEXEC was not open in the command");
	pc_check(status_of(a) != 0, "the descriptor with FD_CLOEXEC was still open in the command");
	pc_pass();
}
