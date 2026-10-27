/*
 * posix_spawnattr_setsigdefault() with a set of SIGUSR1, SIGTERM and SIGRTMIN returns 0...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getsigdefault.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <spawn.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* A descriptor number that is in range and not open. */
static int free_fd_plain(void)
{
	int f = open("/dev/null", O_RDONLY);

	if (f == -1)
		pc_unresolved("cannot open /dev/null");
	(void)close(f);
	return f;
}

static int sigs[40];
static int nsigs;

static void fill_sigs(void)
{
	static const int base[] = { SIGHUP, SIGINT, SIGQUIT, SIGABRT, SIGALRM, SIGTERM, SIGUSR1, SIGUSR2,
	    SIGCHLD, SIGCONT, SIGPIPE, SIGTSTP, SIGTTIN, SIGTTOU, SIGBUS, SIGFPE, SIGILL, SIGSEGV };
	size_t i;

	nsigs = 0;
	for (i = 0; i < sizeof base / sizeof base[0]; i++)
		sigs[nsigs++] = base[i];
#ifdef SIGRTMIN
	sigs[nsigs++] = SIGRTMIN;
	sigs[nsigs++] = SIGRTMAX;
#endif
}

/* Exactly the signals in want[0..nw) are in the set, among those the test knows. */
static void expect_members(const sigset_t *s, const int *want, int nw, const char *what)
{
	int i, j;

	for (i = 0; i < nsigs; i++) {
		int in = 0;

		for (j = 0; j < nw; j++)
			if (want[j] == sigs[i])
				in = 1;
		pc_check(sigismember(s, sigs[i]) == in, "%s: signal %d is %s the set", what, sigs[i], in ? "missing from" : "in");
	}
}

int main(void)
{
	pc_begin("XSH.posix_spawnattr_getsigdefault.002");

	posix_spawnattr_t a, b;
	sigset_t s, t, got;
	int want[3], nw = 0, r;

	fill_sigs();
	sigemptyset(&s);
	sigaddset(&s, SIGUSR1);
	sigaddset(&s, SIGTERM);
	want[nw++] = SIGUSR1;
	want[nw++] = SIGTERM;
#ifdef SIGRTMIN
	sigaddset(&s, SIGRTMIN);
	want[nw++] = SIGRTMIN;
#endif
	sigemptyset(&t);
	sigaddset(&t, SIGUSR2);
	pc_check(posix_spawnattr_init(&a) == 0 && posix_spawnattr_init(&b) == 0, "posix_spawnattr_init failed");
	pc_test();
	r = posix_spawnattr_setsigdefault(&a, &s);
	pc_check(r == 0, "setsigdefault() returned %s", pc_errname(r));
	r = posix_spawnattr_setsigdefault(&b, &t);
	pc_check(r == 0, "setsigdefault() on the second object returned %s", pc_errname(r));
	sigfillset(&got);
	r = posix_spawnattr_getsigdefault(&a, &got);
	pc_check(r == 0, "getsigdefault() returned %s", pc_errname(r));
	expect_members(&got, want, nw, "first object");
	sigfillset(&got);
	r = posix_spawnattr_getsigdefault(&b, &got);
	pc_check(r == 0, "getsigdefault() on the second object returned %s", pc_errname(r));
	{
		int w2[1];
		w2[0] = SIGUSR2;
		expect_members(&got, w2, 1, "second object");
	}
	(void)posix_spawnattr_destroy(&a);
	(void)posix_spawnattr_destroy(&b);
	pc_pass();
}
