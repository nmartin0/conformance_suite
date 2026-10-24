/*
 * The alternate stack belongs to the thread that set it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaltstack.012
 * MUTANT    sigaltstack.borrows_main
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

static char *base;
static size_t size;
static volatile sig_atomic_t ran, on_main;

static void h(int s)
{
	char c;

	(void)s;
	on_main = &c >= base && &c < base + size;
	ran = 1;
}

static void *thr(void *a)
{
	stack_t o;

	(void)a;
	if (sigaltstack(NULL, &o) != 0)
		return (void *)1;
	if (raise(SIGUSR1) != 0 || !ran)
		return (void *)4;
	return on_main ? (void *)3 : NULL;
}

int main(void)
{
	pc_begin("XSH.sigaltstack.012");
	{
		struct sigaction sa;
		stack_t ss;
		pthread_t t;
		void *res;

		size = 4 * (size_t)SIGSTKSZ;
		base = malloc(size);
		pc_check(base != NULL, "malloc failed");
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = h;
		sa.sa_flags = SA_ONSTACK;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed");
		pc_test();
		ss.ss_sp = base;
		ss.ss_size = size;
		ss.ss_flags = 0;
		pc_check(sigaltstack(&ss, NULL) == 0, "sigaltstack() failed: %s", pc_errname(errno));
		pc_check(pthread_create(&t, NULL, thr, NULL) == 0, "pthread_create failed");
		pc_check(pthread_join(t, &res) == 0, "pthread_join failed");
		pc_check(res != (void *)1, "sigaltstack(NULL, &old) failed in the new thread");
		pc_check(res != (void *)2, "the new thread sees the main thread's alternate stack");
		pc_check(res != (void *)4, "the handler did not run in the new thread");
		pc_check(res != (void *)3, "the new thread's handler ran on the main thread's alternate stack");
		pc_check(res == NULL, "unexpected result from the thread");
	}
	pc_pass();
}
