/*
 * fcntl() on a shared memory object behaves as on a regular file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.044
 * MUTANT    fcntl.dupfd_ignores_arg
 * MUTANT    fcntl.setfd_ignored
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
	pc_begin("XSH.fcntl.044");
	{
		char name[64];
		int fd, d, low;

		snprintf(name, sizeof name, "/pc-shmf-%ld", (long)getpid());
		(void)shm_unlink(name);
		fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
		pc_check(fd >= 0, "shm_open failed: %s", pc_errname(errno));
		(void)shm_unlink(name);
		pc_test();
		pc_check(fcntl(fd, F_SETFD, FD_CLOEXEC) == 0, "F_SETFD failed: %s", pc_errname(errno));
		pc_check((fcntl(fd, F_GETFD) & FD_CLOEXEC) != 0, "F_GETFD does not report FD_CLOEXEC");
		pc_check(fcntl(fd, F_SETFD, 0) == 0 && (fcntl(fd, F_GETFD) & FD_CLOEXEC) == 0, "FD_CLOEXEC could not be cleared");
		low = fd + 10;
		if (fcntl(low, F_GETFD) != -1)
			pc_unresolved("descriptor %d is in use", low);
		d = fcntl(fd, F_DUPFD, low);
		pc_check(d == low, "F_DUPFD returned %d, not the lowest free descriptor %d", d, low);
		pc_check((fcntl(fd, F_GETFL) & O_ACCMODE) == O_RDWR, "F_GETFL does not report O_RDWR");
	}
	pc_pass();
}
