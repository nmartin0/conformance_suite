/*
 * An asynchronous write not canceled completes although close() was called.
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

int main(void)
{
	pc_begin("XSH.close.014");
	{
		static char buf[65536];
		static struct aiocb cb;
		struct stat sb;
		int fd, e = EINPROGRESS, i;
		ssize_t r;

		fd = open("f", O_WRONLY | O_CREAT | O_EXCL, 0600);
		pc_check(fd >= 0, "open failed: %s", pc_errname(errno));
		memset(buf, 'q', sizeof buf);
		cb.aio_fildes = fd;
		cb.aio_buf = buf;
		cb.aio_nbytes = sizeof buf;
		cb.aio_sigevent.sigev_notify = SIGEV_NONE;
		pc_test();
		pc_check(aio_write(&cb) == 0, "aio_write failed: %s", pc_errname(errno));
		pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
		for (i = 0; i < 300 && (e = aio_error(&cb)) == EINPROGRESS; i++)
			sleep_ms(10);
		if (e == ECANCELED)
			pc_pass();
		pc_check(e == 0, "aio_error() reports %s", pc_errname(e));
		r = aio_return(&cb);
		pc_check(r == (ssize_t)sizeof buf, "aio_return() gives %ld", (long)r);
		pc_check(stat("f", &sb) == 0 && sb.st_size == 65536, "the file does not hold the data");
	}
	pc_pass();
}
