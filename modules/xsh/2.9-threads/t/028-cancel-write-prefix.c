/*
 * When a cancellation interrupts a write(), what was written is a prefix
 * of the data, and it is all in the pipe before the cleanup handler runs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.028
 *
 * The thread writes 8 MiB of a known pattern to a pipe that nobody reads,
 * so that write() blocks with part of the data in the pipe. After the
 * request, the cleanup handler reads whatever is in the pipe (read end
 * non-blocking); when the thread has ended, nothing more may be in the
 * pipe, and everything read must be the beginning of the pattern. The
 * test is not in use if the pipe took all 8 MiB.
 */
#include "pcth.h"
#include <fcntl.h>

#define N (8u * 1024u * 1024u)

static unsigned char pat[N];
static unsigned char got[N];
static size_t glen;
static int pfd[2];

static size_t drain(void)
{
	size_t total = 0;
	ssize_t r;

	for (;;) {
		r = read(pfd[0], got + glen, N - glen);
		if (r <= 0)
			break;
		glen += (size_t)r;
		total += (size_t)r;
		if (glen == N)
			break;
	}
	return total;
}

static void handler(void *a)
{
	(void)a;
	(void)drain();
	flag_set(3, 1);
}

static void *writer(void *a)
{
	(void)a;
	pthread_cleanup_push(handler, NULL);
	flag_set(0, 1);
	if (write(pfd[1], pat, N) == (ssize_t)N)
		flag_set(1, 1);
	pthread_cleanup_pop(0);
	return NULL;
}

int main(void)
{
	pthread_t t;
	void *st;
	size_t i, later;
	int fl;

	pc_begin("XSH.2.9-threads.028");
	guard(120);
	for (i = 0; i < N; i++)
		pat[i] = (unsigned char)(i % 251u);
	NEED(pipe(pfd) == 0, "pipe failed");
	fl = fcntl(pfd[0], F_GETFL);
	NEED(fl != -1 && fcntl(pfd[0], F_SETFL, fl | O_NONBLOCK) == 0, "fcntl failed");
	pc_test();
	t = start(writer, NULL);
	NEED(wait_flag(0, 1, 10000), "the writer did not start");
	sleep_ms(300);
	if (flag_get(1))
		pc_notinuse("the pipe took all 8 MiB, so write() did not block");
	NEED(pthread_cancel(t) == 0, "pthread_cancel failed");
	pc_check(wait_flag(3, 1, 10000), "the thread blocked in write() did not run its cleanup handler within 10 s of the request");
	NEED(join_within(t, 10000, &st), "the thread did not end");
	pc_check(st == PTHREAD_CANCELED, "the thread did not end canceled");
	later = drain();
	pc_check(later == 0, "%lu bytes reached the pipe after the cleanup handler had run", (unsigned long)later);
	if (glen == 0)
		pc_notinuse("write() had transferred nothing when it was canceled, so there is no data to check");
	for (i = 0; i < glen; i++)
		if (got[i] != pat[i])
			pc_fail("the data in the pipe is not the beginning of what was written (first difference at byte %lu)", (unsigned long)i);
	pc_pass();
}
