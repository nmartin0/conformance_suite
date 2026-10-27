/*
 * Parent and command run independently.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.012
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

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* The file's contents (up to 255 bytes) as a string; "" if it cannot be read. */
static const char *slurp_file(const char *path)
{
	static char b[256];
	int f = open(path, O_RDONLY);
	ssize_t n;

	b[0] = '\0';
	if (f < 0)
		return b;
	n = read(f, b, sizeof b - 1);
	(void)close(f);
	if (n > 0)
		b[n] = '\0';
	else
		b[0] = '\0';
	return b;
}
static void finish(FILE *f)
{
	if (pclose(f) == -1)
		pc_unresolved("pclose() failed: %s", pc_errname(errno));
}

int main(void)
{
	FILE *f;
	int i;

	pc_begin("XSH.popen.012");
	guard(30);
	pc_test();
	errno = 0;
	f = popen("cat -u > out", "w");	/* does not end until its input does */
	pc_check(f != NULL, "popen() failed: %s", pc_errname(errno));
	pc_check(fputs("x\n", f) >= 0 && fflush(f) == 0, "writing to the stream failed");
	for (i = 0; i < 200 && strcmp(slurp_file("out"), "x\n") != 0; i++)
		sleep_ms(25);
	pc_check(strcmp(slurp_file("out"), "x\n") == 0, "the running command did not receive the data while the stream was still open");
	finish(f);
	pc_pass();
}
