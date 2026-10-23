/*
 * Asynchronous writes not canceled at _exit() still complete, without their signal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.019
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
	pc_begin("XSH._Exit.019");
	{
		struct stat sb;
		pid_t kid;
		int st, i;

		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0) {
			static char buf[65536];
			static struct aiocb cb;
			int fd = open("f", O_WRONLY | O_CREAT | O_EXCL, 0600);
			if (fd < 0)
				_exit(99);
			memset(buf, 'z', sizeof buf);
			cb.aio_fildes = fd;
			cb.aio_buf = buf;
			cb.aio_nbytes = sizeof buf;
			cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
			cb.aio_sigevent.sigev_signo = SIGUSR1;
			if (aio_write(&cb) != 0)
				_exit(98);
			_exit(0);
		}
		pc_check(waitpid(kid, &st, 0) == kid, "waitpid failed");
		if (WIFEXITED(st) && WEXITSTATUS(st) >= 98)
			pc_unresolved("the child could not start the write (%d)", WEXITSTATUS(st));
		pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child did not exit with 0 (status %#x): a signal was not suppressed",
			 (unsigned)st);
		for (i = 0; i < 300; i++) {
			if (stat("f", &sb) == 0 && sb.st_size == 65536)
				break;
			sleep_ms(10);
		}
		pc_check(stat("f", &sb) == 0, "stat failed");
		pc_check(sb.st_size == 65536, "the file holds %lld bytes, not 65536", (long long)sb.st_size);
	}
	pc_pass();
}
