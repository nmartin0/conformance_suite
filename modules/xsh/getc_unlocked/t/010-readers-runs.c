/*
 * getc_unlocked() under flockfile() in two threads: each thread reads an unbroken run.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getc_unlocked.010
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
#include <sched.h>
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

/* Creates path holding the n bytes of data (setup). */
static void make_bytes(const char *path, const void *data, size_t n)
{
	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	pc_check(write(fd, data, n) == (ssize_t)n, "write(%s) failed: %s", path, pc_errname(errno));
	pc_check(close(fd) == 0, "close(%s) failed: %s", path, pc_errname(errno));
}

/* A call that blocks must fail the test, not hang it. */
static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: the call under test did not finish in time");
}

/*
 * Both threads start together and give the processor up inside the locked
 * sequence, so that a lock that does not exclude would let the other
 * thread in (else one thread could finish before the other begins).
 */
static atomic_int nready;
static atomic_int go;

static void rendezvous(void)
{
	atomic_fetch_add(&nready, 1);
	while (atomic_load(&go) == 0)
		sched_yield();
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

struct rarg {
	FILE *f;
	int got[500];
	int bad;
};

static void *reader(void *p)
{
	struct rarg *a = p;
	int i, c;

	rendezvous();
	flockfile(a->f);
	for (i = 0; i < 500; i++) {
		c = getc_unlocked(a->f);
		if (c == EOF) {
			a->bad = 1;
			break;
		}
		a->got[i] = c;
		sched_yield();
	}
	funlockfile(a->f);
	return NULL;
}

int main(void)
{
	pc_begin("XSH.getc_unlocked.010");
	{
		static unsigned char data[1000];
		struct rarg a[2];
		pthread_t t[2];
		FILE *f;
		int i, j, r, lo, hi;

		for (i = 0; i < 1000; i++)
			data[i] = (unsigned char)(i % 251);
		make_bytes("f", data, sizeof data);
		guard(60);
		pc_test();
		f = open_as("f", "r");
		for (i = 0; i < 2; i++) {
			a[i].f = f;
			a[i].bad = 0;
			r = pthread_create(&t[i], NULL, reader, &a[i]);
			NEED(r == 0, "pthread_create failed: %s", pc_errname(r));
		}
		while (atomic_load(&nready) < 2)
			sched_yield();
		atomic_store(&go, 1);
		for (i = 0; i < 2; i++)
			NEED(pthread_join(t[i], NULL) == 0, "pthread_join failed");
		for (i = 0; i < 2; i++) {
			pc_check(a[i].bad == 0, "thread %d read EOF or an error before 500 bytes", i + 1);
			for (j = 1; j < 500; j++)
				pc_check(a[i].got[j] == (a[i].got[j - 1] + 1) % 251, "thread %d: byte %d is %d after %d: its run was broken by the other thread", i + 1, j, a[i].got[j], a[i].got[j - 1]);
		}
		lo = a[0].got[0] < a[1].got[0] ? 0 : 1;
		hi = 1 - lo;
		pc_check(a[lo].got[0] == 0 && a[hi].got[0] == 500 % 251, "the runs start at %d and %d, expected 0 and %d", a[lo].got[0], a[hi].got[0], 500 % 251);
		(void)fclose(f);
	}
	pc_pass();
}
