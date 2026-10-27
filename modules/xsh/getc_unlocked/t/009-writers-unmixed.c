/*
 * putc_unlocked() under flockfile() in two threads: each thread's run is unbroken.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getc_unlocked.009
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

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

/* Up to 16 bytes as hex, for messages. */
static const char *hex(const void *p, size_t n)
{
	static char out[16 * 3 + 1];
	const unsigned char *b = p;
	size_t i;

	out[0] = '\0';
	for (i = 0; i < n && i < 16; i++)
		snprintf(out + 3 * i, sizeof out - 3 * i, "%02x ", b[i]);
	return out;
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

struct warg {
	FILE *f;
	int ch;
	int bad;
};

static void *writer(void *p)
{
	struct warg *a = p;
	int i;

	rendezvous();
	flockfile(a->f);
	for (i = 0; i < 500; i++) {
		if (putc_unlocked(a->ch, a->f) != a->ch)
			a->bad = 1;
		sched_yield();
	}
	funlockfile(a->f);
	return NULL;
}

int main(void)
{
	pc_begin("XSH.getc_unlocked.009");
	{
		static char got[1100];
		struct warg a[2];
		pthread_t t[2];
		FILE *f;
		long n;
		int i, r;

		guard(60);
		pc_test();
		f = open_as("f", "w+");
		for (i = 0; i < 2; i++) {
			a[i].f = f;
			a[i].ch = 'A' + i;
			a[i].bad = 0;
			r = pthread_create(&t[i], NULL, writer, &a[i]);
			NEED(r == 0, "pthread_create failed: %s", pc_errname(r));
		}
		while (atomic_load(&nready) < 2)
			sched_yield();
		atomic_store(&go, 1);
		for (i = 0; i < 2; i++)
			NEED(pthread_join(t[i], NULL) == 0, "pthread_join failed");
		pc_check(a[0].bad == 0 && a[1].bad == 0, "putc_unlocked() returned a wrong value or failed");
		NEED(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		n = slurp("f", got, sizeof got);
		pc_check(n == 1000, "the file holds %ld bytes, expected 1000", n);
		for (i = 1; i < 500; i++)
			pc_check(got[i] == got[0] && got[500 + i] == got[500], "the bytes of the two threads are mixed (at offset %d)", i);
		pc_check((got[0] == 'A' && got[500] == 'B') || (got[0] == 'B' && got[500] == 'A'), "the runs are %d and %d, expected 'A' and 'B' in either order", got[0], got[500]);
		(void)fclose(f);
	}
	pc_pass();
}
