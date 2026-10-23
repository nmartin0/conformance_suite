/*
 * Blocking read-only FIFO open waits for a writer; a caught signal gives EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.029
 * MUTANT    open.fifo_never_blocks
 * MUTANT    open.eintr_as_eagain
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static void on_alarm(int sig)
{
	(void)sig;
}

/* SIGALRM is caught, without SA_RESTART, so it interrupts a blocked call. */
static void catch_alarm(void)
{
	struct sigaction sa;

	sa.sa_handler = on_alarm;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
}

static long ms_since(const struct timespec *t0)
{
	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC, &t);
	return (t.tv_sec - t0->tv_sec) * 1000L +
	    (t.tv_nsec - t0->tv_nsec) / 1000000L;
}

int main(void)
{
	struct timespec t0;
	long ms;
	int fd;

	pc_begin("XSH.open.029");
	pc_check(mkfifo("p", 0600) == 0, "mkfifo failed");
	catch_alarm();
	pc_test();
	clock_gettime(CLOCK_MONOTONIC, &t0);
	alarm(1);
	fd = open("p", O_RDONLY);
	ms = ms_since(&t0);
	pc_check(fd == -1, "open() returned a descriptor although no writer exists");
	pc_expect_err(fd, EINTR, "blocked open(\"p\", O_RDONLY) interrupted by SIGALRM");
	pc_check(ms >= 500, "open() returned after %ld ms, before the signal: it did not block", ms);
	pc_pass();
}
