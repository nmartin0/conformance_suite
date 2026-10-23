/*
 * An asynchronous I/O completion signal has si_code SI_ASYNCIO.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.4.3-signal-actions.018
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

static volatile sig_atomic_t got;
static volatile int code, val, signo;

static void h(int s, siginfo_t *i, void *u)
{
	(void)u;
	signo = s;
	code = i->si_code;
	val = i->si_value.sival_int;
	got = 1;
}

static void catch_info(int sig)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = h;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction failed");
}

int main(void)
{
	pc_begin("XSH.2.4.3-signal-actions.018");
	{
		static char data[] = "hello";
		struct aiocb cb;
		int fd;

		catch_info(SIGRTMIN);
		fd = open("f", O_RDWR | O_CREAT | O_EXCL, 0600);
		pc_check(fd >= 0, "open failed: %s", pc_errname(errno));
		memset(&cb, 0, sizeof cb);
		cb.aio_fildes = fd;
		cb.aio_buf = data;
		cb.aio_nbytes = 5;
		cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
		cb.aio_sigevent.sigev_signo = SIGRTMIN;
		cb.aio_sigevent.sigev_value.sival_int = 88;
		pc_test();
		pc_check(aio_write(&cb) == 0, "aio_write failed: %s", pc_errname(errno));
		pc_check(wait_for(&got, 3000), "the handler did not run");
		pc_check(code == SI_ASYNCIO, "si_code is %d, expected SI_ASYNCIO (%d)", code, SI_ASYNCIO);
		pc_check(val == 88, "si_value is %d, expected 88", val);
	}
	pc_pass();
}
