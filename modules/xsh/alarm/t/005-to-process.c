/*
 * SIGALRM from alarm() is for the process: another thread may take it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.alarm.005
 * MUTANT    alarm.thread_directed
 * VARIANT   alarm.early_half
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero. */
static int wait_for(volatile sig_atomic_t *flag, long ms)
{
	long i;

	for (i = 0; i < ms / 10 && !*flag; i++)
		sleep_ms(10);
	return *flag != 0;
}

static long ms_since(const struct timespec *t0)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long)(t.tv_sec - t0->tv_sec) * 1000 + (t.tv_nsec - t0->tv_nsec) / 1000000;
}

static pthread_t who;
static volatile sig_atomic_t got;

static void h(int s)
{
	(void)s;
	who = pthread_self();
	got = 1;
}

static void *thr(void *a)
{
	sigset_t none;

	(void)a;
	sigemptyset(&none);
	(void)pthread_sigmask(SIG_SETMASK, &none, NULL);
	(void)wait_for(&got, 3000);
	return NULL;
}

int main(void)
{
	pc_begin("XSH.alarm.005");
	{
		struct sigaction sa;
		sigset_t one;
		pthread_t t;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = h;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
		sigemptyset(&one);
		sigaddset(&one, SIGALRM);
		pc_check(pthread_sigmask(SIG_BLOCK, &one, NULL) == 0, "pthread_sigmask failed");
		pc_check(pthread_create(&t, NULL, thr, NULL) == 0, "pthread_create failed");
		pc_test();
		(void)alarm(1);
		pc_check(pthread_join(t, NULL) == 0, "pthread_join failed");
		(void)alarm(0);
		pc_check(got, "SIGALRM did not reach the thread that does not block it");
		pc_check(pthread_equal(who, t), "the handler ran in another thread");
	}
	pc_pass();
}
