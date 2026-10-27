/*
 * popen() fails with EMFILE when {STREAM_MAX} streams are open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.014
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

#include <sys/resource.h>

/* Opens streams until {STREAM_MAX} are open (stdin, stdout and stderr count). */
static void fill_streams(void)
{
	struct rlimit rl;
	long max, i;

	errno = 0;
	max = sysconf(_SC_STREAM_MAX);
	if (max == -1 && errno == 0)
		pc_notinuse("{STREAM_MAX} is indeterminate");
	if (max < 8)
		pc_unresolved("sysconf(_SC_STREAM_MAX) gave %ld", max);
	if (max > 4096) {
		/* It may follow RLIMIT_NOFILE: see whether a lower limit lowers it. */
		pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
		rl.rlim_cur = 256;
		if (setrlimit(RLIMIT_NOFILE, &rl) == 0 && sysconf(_SC_STREAM_MAX) != max)
			pc_notinuse("{STREAM_MAX} follows RLIMIT_NOFILE; running out of descriptors is another case");
		pc_unresolved("{STREAM_MAX} is %ld, too many streams to open here", max);
	}
	pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	if (rl.rlim_cur != RLIM_INFINITY && rl.rlim_cur < (rlim_t)max + 16) {
		if (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < (rlim_t)max + 16)
			pc_unresolved("RLIMIT_NOFILE (%ld) is below {STREAM_MAX} (%ld)", (long)rl.rlim_max, max);
		rl.rlim_cur = (rlim_t)max + 16;
		if (setrlimit(RLIMIT_NOFILE, &rl) != 0)
			pc_unresolved("setrlimit failed");
		if (sysconf(_SC_STREAM_MAX) != max)
			pc_notinuse("{STREAM_MAX} follows RLIMIT_NOFILE; running out of descriptors is another case");
	}
	for (i = 3; i < max; i++) {
		errno = 0;
		if (fopen("s", "r") == NULL)
			pc_unresolved("only %ld streams could be opened, below {STREAM_MAX} (%ld): %s", i, max, pc_errname(errno));
	}
}

int main(void)
{
	FILE *f;
	int e;

	pc_begin("XSH.popen.014");
	guard(20);
	pc_make_file("s", "x");
	fill_streams();	/* before the test phase */
	pc_test();
	errno = 0;
	f = popen("true", "r");
	e = errno;
	if (f != NULL)
		(void)pclose(f);
	pc_check(f == NULL, "popen() succeeded with {STREAM_MAX} streams open");
	pc_check(e == EMFILE, "popen() failed with %s, expected EMFILE", pc_errname(e));
	pc_pass();
}
