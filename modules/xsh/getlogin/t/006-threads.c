/*
 * getlogin_r() is thread-safe.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlogin.006
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
#include <pthread.h>

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

static int expected;
static char expected_name[256];
static int bad[4];

static void *run(void *arg)
{
	long k = (long)arg;
	char buf[256];
	int i, r;

	for (i = 0; i < 500; i++) {
		memset(buf, 'Z', sizeof buf);
		r = getlogin_r(buf, sizeof buf);
		if (expected == 0 ? (r != 0 || strncmp(buf, expected_name, sizeof buf) != 0) : r <= 0)
			bad[k]++;
	}
	return NULL;
}

int main(void)
{
	pthread_t t[4];
	long k;

	pc_begin("XSH.getlogin.006");
	guard(60);
	expected = getlogin_r(expected_name, sizeof expected_name);
	pc_test();
	for (k = 0; k < 4; k++)
		if (pthread_create(&t[k], NULL, run, (void *)k) != 0)
			pc_unresolved("pthread_create failed");
	for (k = 0; k < 4; k++)
		(void)pthread_join(t[k], NULL);
	for (k = 0; k < 4; k++)
		pc_check(bad[k] == 0, "thread %ld got a different result from the main thread's in %d of 500 calls", k, bad[k]);
	pc_pass();
}
