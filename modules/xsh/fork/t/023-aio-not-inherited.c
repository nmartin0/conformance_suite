/*
 * A pending asynchronous read does not complete into the child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.023
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

static char buf[8];

int main(void)
{
	pc_begin("XSH.fork.023");
	{
		static struct aiocb cb;
		int p[2], q[2], st, e = EINPROGRESS, i;
		pid_t kid;
		char c;

		pc_check(pipe(p) == 0 && pipe(q) == 0, "pipe failed");
		memset(buf, 0, sizeof buf);
		cb.aio_fildes = p[0];
		cb.aio_buf = buf;
		cb.aio_nbytes = 4;
		cb.aio_sigevent.sigev_notify = SIGEV_NONE;
		pc_check(aio_read(&cb) == 0, "aio_read failed: %s", pc_errname(errno));
		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0) {
			(void)read(q[0], &c, 1);
			sleep_ms(200);
			_exit(buf[0] == 0 ? 0 : 1);
		}
		pc_check(write(p[1], "data", 4) == 4, "write failed");
		for (i = 0; i < 300 && (e = aio_error(&cb)) == EINPROGRESS; i++)
			sleep_ms(10);
		(void)write(q[1], "g", 1);
		pc_check(waitpid(kid, &st, 0) == kid && WIFEXITED(st), "the child did not exit");
		pc_check(e == 0 && memcmp(buf, "data", 4) == 0, "the parent's read did not complete (%s)", pc_errname(e));
		pc_check(WEXITSTATUS(st) == 0, "the read completed into the child's memory");
	}
	pc_pass();
}
