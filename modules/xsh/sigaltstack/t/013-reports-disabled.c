/*
 * sigaltstack() reports SS_DISABLE after disabling, and in a new thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaltstack.013
 * MUTANT    sigaltstack.disable_ignored
 * MUTANT    sigaltstack.process_wide
 * VARIANT   sigaltstack.bsd_flags
 * VARIANT   sigaltstack.minsigstksz_32k
 * VARIANT   sigaltstack.disable_size_checked
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

static void *thr13(void *a)
{
	stack_t o;

	(void)a;
	if (sigaltstack(NULL, &o) != 0)
		return (void *)1;
	return (o.ss_flags & SS_DISABLE) ? NULL : (void *)2;
}

int main(void)
{
	stack_t s, d, o;
	pthread_t t;
	void *r;
	size_t sz = 4 * (size_t)MINSIGSTKSZ + 65536;
	char *alt;

	pc_begin("XSH.sigaltstack.013");
	alt = malloc(sz);
	pc_check(alt != NULL, "malloc failed");
	s.ss_sp = alt;
	s.ss_size = sz;
	s.ss_flags = 0;
	d = s;	/* a valid size: whether it is ignored is XSH.sigaltstack.011 */
	d.ss_flags = SS_DISABLE;
	pc_test();
	pc_check(sigaltstack(&s, NULL) == 0, "sigaltstack() failed: %s", pc_errname(errno));
	pc_check(pthread_create(&t, NULL, thr13, NULL) == 0 && pthread_join(t, &r) == 0, "the thread could not run");
	pc_check(r != (void *)1, "sigaltstack(NULL, &o) failed in the new thread");
	pc_check(r == NULL, "a new thread's sigaltstack() does not report SS_DISABLE");
	pc_check(sigaltstack(&d, NULL) == 0, "sigaltstack(SS_DISABLE) failed: %s", pc_errname(errno));
	pc_check(sigaltstack(NULL, &o) == 0, "sigaltstack(NULL, &o) failed");
	pc_check(o.ss_flags & SS_DISABLE, "after SS_DISABLE, ss_flags is %#x", o.ss_flags);
	pc_pass();
}
