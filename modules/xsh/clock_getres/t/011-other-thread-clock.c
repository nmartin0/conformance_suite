/*
 * pthread_getcpuclockid() gives another thread's clock, readable from here.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.011
 * MUTANT    clock_getres.foreign_is_self
 * MUTANT    clock_getres.cpuclockid_einval
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static int go[2], done[2], quit[2];
static long long s0, s1;

static long long ns_of(clockid_t id)
{
	struct timespec t;

	if (clock_gettime(id, &t) != 0 || t.tv_nsec < 0 || t.tv_nsec >= 1000000000L)
		return -1;
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static void *worker(void *arg)
{
	volatile unsigned long spin = 0;
	long long w0, c;
	char ch;

	(void)arg;
	if (read(go[0], &ch, 1) != 1)
		return NULL;
	s0 = ns_of(CLOCK_THREAD_CPUTIME_ID);
	w0 = ns_of(CLOCK_MONOTONIC);
	do {
		int k;
		for (k = 0; k < 100000; k++)
			spin++;
		c = ns_of(CLOCK_THREAD_CPUTIME_ID);
	} while (s0 >= 0 && c >= 0 && c - s0 < 200000000LL && ns_of(CLOCK_MONOTONIC) - w0 < 20000000000LL);
	s1 = c;
	(void)write(done[1], "d", 1);
	(void)read(quit[0], &ch, 1);	/* stay alive while main reads the clock */
	return NULL;
}

int main(void)
{
	pthread_t t;
	clockid_t id;
	long long v0, v1;
	char ch;
	int r;

	pc_begin("XSH.clock_getres.011");
	NEED(pipe(go) == 0 && pipe(done) == 0 && pipe(quit) == 0, "pipe failed");
	r = pthread_create(&t, NULL, worker, NULL);
	NEED(r == 0, "pthread_create failed: %s", pc_errname(r));
	r = pthread_getcpuclockid(t, &id);
	NEED(r == 0, "pthread_getcpuclockid failed: %s", pc_errname(r));
	pc_test();
	v0 = ns_of(id);
	pc_check(v0 >= 0, "clock_gettime(the other thread's clock ID) failed: %s", pc_errname(errno));
	NEED(write(go[1], "g", 1) == 1, "write failed");
	NEED(read(done[0], &ch, 1) == 1, "the other thread did not finish");
	v1 = ns_of(id);
	(void)write(quit[1], "q", 1);
	(void)pthread_join(t, NULL);
	NEED(s0 >= 0 && s1 >= 0, "the other thread could not read its own CPU-time clock");
	NEED(s1 - s0 >= 200000000LL, "the other thread's own clock did not reach 200 ms in 20 s");
	pc_check(v1 >= 0, "clock_gettime(the other thread's clock ID) failed: %s", pc_errname(errno));
	pc_check(v1 - v0 >= s1 - s0 - 50000000LL,
	    "read from the main thread, the other thread's clock advanced %lld ms while its own reading advanced %lld ms",
	    (v1 - v0) / 1000000LL, (s1 - s0) / 1000000LL);
	pc_pass();
}
