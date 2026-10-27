/*
 * Without 'e', FD_CLOEXEC is clear.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.007
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

static void check(const char *cmd, const char *mode)
{
	FILE *f;
	int flags;

	errno = 0;
	f = popen(cmd, mode);
	pc_check(f != NULL, "popen(\"%s\", \"%s\") failed: %s", cmd, mode, pc_errname(errno));
	flags = fcntl(fileno(f), F_GETFD);
	pc_check(flags != -1, "fcntl(F_GETFD) failed: %s", pc_errname(errno));
	pc_check(!(flags & FD_CLOEXEC), "FD_CLOEXEC is set on the descriptor under a popen(..., \"%s\") stream", mode);
	finish(f);
}

int main(void)
{
	pc_begin("XSH.popen.007");
	guard(20);
	pc_test();
	check("true", "r");
	check("cat > /dev/null", "w");
	pc_pass();
}
