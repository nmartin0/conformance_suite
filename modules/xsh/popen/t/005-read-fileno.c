/*
 * With "r", fileno() is the readable end.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.005
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

int main(void)
{
	char b[16];
	size_t got = 0;
	ssize_t n;
	int fd;
	FILE *f;

	pc_begin("XSH.popen.005");
	guard(20);
	pc_test();
	errno = 0;
	f = popen("echo hello", "r");
	pc_check(f != NULL, "popen() failed: %s", pc_errname(errno));
	fd = fileno(f);
	pc_check(fd >= 0, "fileno() failed");
	while (got < 6) {
		n = read(fd, b + got, sizeof b - got);
		pc_check(n > 0 || (n < 0 && errno == EINTR), "read() on the descriptor returned %ld (%s) after %d bytes", (long)n, pc_errname(errno), (int)got);
		if (n > 0)
			got += (size_t)n;
	}
	pc_check(memcmp(b, "hello\n", 6) == 0, "read() returned \"%.6s\", expected \"hello\\n\"", b);
	finish(f);
	pc_pass();
}
