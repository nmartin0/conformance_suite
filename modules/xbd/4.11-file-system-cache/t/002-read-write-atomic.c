/*
 * A read that runs in one thread while another thread rewrites the same
 * bytes with a single write sees either all of the old data or all of the
 * new, never a mixture.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.11-file-system-cache.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define N 262144
#define MINOPS 2000

static int fd;
static volatile int stop;
static volatile long writes;
static char *wa, *wb;

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

static void *writer(void *arg)
{
	long i = 0;

	(void)arg;
	while (!stop) {
		if (pwrite(fd, (i & 1) ? wb : wa, N, 0) != N)
			break;
		i++;
		writes = i;
	}
	return NULL;
}

int main(void)
{
	pthread_t tw;
	char *rb;
	long reads = 0, torn = 0, sawa = 0, sawb = 0, i;
	struct timespec t0, t1;
	ssize_t n;

	pc_begin("XBD.4.11-file-system-cache.002");
	(void)signal(SIGALRM, on_alarm);
	wa = malloc(N);
	wb = malloc(N);
	rb = malloc(N);
	if (wa == NULL || wb == NULL || rb == NULL)
		pc_unresolved("malloc failed");
	memset(wa, 'A', N);
	memset(wb, 'B', N);
	fd = open("blk", O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd < 0 || pwrite(fd, wa, N, 0) != N)
		pc_unresolved("cannot make the file: %s", pc_errname(errno));
	pc_test();
	(void)alarm(100);
	if (pthread_create(&tw, NULL, writer, NULL) != 0)
		pc_unresolved("pthread_create failed");
	(void)clock_gettime(CLOCK_MONOTONIC, &t0);
	do {
		n = pread(fd, rb, N, 0);
		if (n != N)
			pc_unresolved("a read of %d bytes from a regular file returned %ld", N, (long)n);
		reads++;
		for (i = 1; i < N; i++)
			if (rb[i] != rb[0])
				break;
		if (i < N || (rb[0] != 'A' && rb[0] != 'B'))
			torn++;
		else if (rb[0] == 'A')
			sawa++;
		else
			sawb++;
		(void)clock_gettime(CLOCK_MONOTONIC, &t1);
	} while (torn == 0 && ((t1.tv_sec - t0.tv_sec) * 1000L + (t1.tv_nsec - t0.tv_nsec) / 1000000L < 1500 || reads < MINOPS));
	stop = 1;
	(void)pthread_join(tw, NULL);
	pc_check(torn == 0, "a read of %d bytes, concurrent with writes of %d bytes, returned data that is neither all old nor all new (or fewer bytes)", N, N);
	if (sawa == 0 || sawb == 0 || writes < 10)
		pc_unresolved("the reader never overlapped the writer (%ld reads, %ld writes): nothing was exercised", reads, (long)writes);
	pc_pass();
}
