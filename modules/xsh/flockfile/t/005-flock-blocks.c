/*
 * flockfile() by another thread waits while a stream is owned.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.flockfile.005
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

struct targ {
	FILE *f;
	int result;
	atomic_int flag;
	atomic_int started;
};

static void *try_thread(void *p)
{
	struct targ *a = p;

	a->result = ftrylockfile(a->f);
	if (a->result == 0)
		funlockfile(a->f);
	return NULL;
}

/* ftrylockfile(f) as called by another thread (it never waits): its return value. */
static int try_elsewhere(FILE *f)
{
	struct targ a;
	pthread_t t;
	int r;

	a.f = f;
	a.result = -1;
	atomic_init(&a.flag, 0);
	r = pthread_create(&t, NULL, try_thread, &a);
	NEED(r == 0, "pthread_create failed: %s", pc_errname(r));
	r = pthread_join(t, NULL);
	NEED(r == 0, "pthread_join failed: %s", pc_errname(r));
	return a.result;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

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

/* Waits for *flag to become 1 (the lock ought to be given up at once): 10 s at most. */
static void wait_flag(atomic_int *flag)
{
	int i;

	for (i = 0; i < 1000 && atomic_load(flag) == 0; i++)
		sleep_ms(10);
	if (atomic_load(flag) == 0)
		pc_unresolved("the waiting thread did not proceed within 10 s of funlockfile(): no time bound is stated");
}

static void *lock_thread(void *p)
{
	struct targ *a = p;

	atomic_store(&a->started, 1);
	flockfile(a->f);
	atomic_store(&a->flag, 1);
	funlockfile(a->f);
	return NULL;
}

int main(void)
{
	pc_begin("XSH.flockfile.005");
	{
		struct targ a;
		pthread_t t;
		FILE *f;
		int r, got;

		guard(60);
		pc_test();
		f = open_as("f", "w+");
		a.f = f;
		atomic_init(&a.flag, 0);
		atomic_init(&a.started, 0);
		flockfile(f);
		r = pthread_create(&t, NULL, lock_thread, &a);
		NEED(r == 0, "pthread_create failed: %s", pc_errname(r));
		/* The thread must be running before the wait counts (else a late start passes vacuously). */
		for (r = 0; r < 1000 && atomic_load(&a.started) == 0; r++)
			sleep_ms(10);
		NEED(atomic_load(&a.started) != 0, "the second thread did not start within 10 s");
		sleep_ms(200);
		got = atomic_load(&a.flag);
		funlockfile(f);
		pc_check(got == 0, "flockfile() by another thread returned while the main thread owned the stream");
		wait_flag(&a.flag);
		NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
		(void)fclose(f);
	}
	pc_pass();
}
