/*
 * system() can be called from several threads at once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.019
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

static int result[4];

static void *run(void *arg)
{
	long k = (long)arg;
	char cmd[32];

	(void)snprintf(cmd, sizeof cmd, "sleep 1; exit %ld", k);
	result[k - 1] = system(cmd);
	return NULL;
}

int main(void)
{
	pthread_t t[4];
	long k;

	pc_begin("XSH.system.019");
	guard(60);
	pc_test();
	for (k = 1; k <= 4; k++)
		if (pthread_create(&t[k - 1], NULL, run, (void *)k) != 0)
			pc_unresolved("pthread_create failed");
	for (k = 1; k <= 4; k++)
		(void)pthread_join(t[k - 1], NULL);
	for (k = 1; k <= 4; k++) {
		pc_check(result[k - 1] != -1, "the call in thread %ld failed", k);
		pc_check(WIFEXITED(result[k - 1]) && WEXITSTATUS(result[k - 1]) == k, "thread %ld got status %#x, expected exit %ld", k, result[k - 1], k);
	}
	pc_pass();
}
