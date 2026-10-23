/*
 * Asynchronous writes not canceled complete although close() was called.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.014
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
#include <aio.h>

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

static int p[2];

/* Fills the pipe (setup). */
static void fill(int w)
{
	static char blk[4096];
	ssize_t r;

	pc_check(fcntl(w, F_SETFL, O_NONBLOCK) == 0, "fcntl failed");
	while ((r = write(w, blk, sizeof blk)) > 0)
		;
	while ((r = write(w, blk, 1)) > 0)
		;
	pc_check(errno == EAGAIN, "could not fill the pipe");
}

static int closing_fd, close_r, close_e;
static volatile sig_atomic_t closed;

static void *closer(void *a)
{
	(void)a;
	close_r = close(closing_fd);
	close_e = errno;
	closed = 1;
	return NULL;
}

static void *drain(void *a)
{
	static char junk[65536];

	(void)a;
	while (read(p[0], junk, sizeof junk) > 0)
		;
	return NULL;
}

int main(void)
{
	pc_begin("XSH.close.014");
	{
		static char a[100000], b[100000];
		static struct aiocb ca, cb;
		const struct aiocb *list[2];
		struct timespec ten = { 10, 0 };
		pthread_t t;
		int fd, i, e;
		ssize_t r;

		pc_check(pipe(p) == 0, "pipe failed");
		fd = p[1];
		fill(fd);
		pc_check(fcntl(fd, F_SETFL, 0) == 0, "fcntl failed");
		memset(a, 'a', sizeof a);
		memset(b, 'b', sizeof b);
		ca.aio_fildes = cb.aio_fildes = fd;
		ca.aio_buf = a;
		cb.aio_buf = b;
		ca.aio_nbytes = cb.aio_nbytes = sizeof a;
		ca.aio_sigevent.sigev_notify = cb.aio_sigevent.sigev_notify = SIGEV_NONE;
		pc_test();
		if (aio_write(&ca) != 0 || aio_write(&cb) != 0) {
			if (errno == EAGAIN)
				pc_unresolved("aio_write() was refused for lack of resources (EAGAIN)");
			pc_check(0, "aio_write failed: %s", pc_errname(errno));
		}
		pc_check(pthread_create(&t, NULL, drain, NULL) == 0, "pthread_create failed");
		closing_fd = fd;
		pc_check(pthread_create(&t, NULL, closer, NULL) == 0, "pthread_create failed");
		for (i = 0; i < 1000 && !closed; i++)
			sleep_ms(10);
		pc_check(closed, "close() had not returned after 10 s");
		pc_check(close_r == 0, "close failed: %s", pc_errname(close_e));
		list[0] = &ca;
		list[1] = &cb;
		for (i = 0; i < 2; i++) {
			const struct aiocb *one[1];
			one[0] = list[i];
			while ((e = aio_error(list[i])) == EINPROGRESS)
				if (aio_suspend(one, 1, &ten) != 0 && errno != EINTR)
					break;
			pc_check(e != EINPROGRESS, "operation %d had not finished after 10 s", i + 1);
			r = aio_return((struct aiocb *)list[i]);
			if (e == ECANCELED)
				continue;
			pc_check(e == 0, "operation %d failed with %s (neither completed nor canceled)", i + 1, pc_errname(e));
			pc_check(r == (ssize_t)sizeof a, "operation %d wrote %ld bytes of %ld", i + 1, (long)r, (long)sizeof a);
		}
	}
	pc_pass();
}
