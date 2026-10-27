/*
 * pclose() waits for the command to end.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pclose.004
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

static long now_ms(void)
{
	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000L + t.tv_nsec / 1000000L;
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
	long t0, t1;
	int r;

	pc_begin("XSH.pclose.004");
	guard(30);
	t0 = now_ms();
	f = start("sleep 1; echo done > f", "r");
	pc_test();
	r = pclose(f);
	t1 = now_ms();
	pc_check(r != -1 && WIFEXITED(r) && WEXITSTATUS(r) == 0, "pclose() gave %d", r);
	pc_check(strcmp(slurp_file("f"), "done\n") == 0, "the file holds \"%s\" when pclose() returned", slurp_file("f"));
	pc_check(t1 - t0 >= 900, "pclose() returned after %ld ms, for a command that sleeps 1 s", t1 - t0);
	pc_pass();
}
