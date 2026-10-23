/*
 * SIGSEGV for an unmapped page has SEGV_MAPERR; SIGBUS a BUS_ code.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.4.3-signal-actions.020
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

static volatile char *target;
static long target_len;
static int last_code;

static void hf(int s, siginfo_t *i, void *u)
{
	int ok_code;
	char *a = i->si_addr;

	(void)u;
	if (s == SIGSEGV)
		ok_code = i->si_code == SEGV_MAPERR;
	else
		ok_code = i->si_code == BUS_ADRERR || i->si_code == BUS_OBJERR;
	if (!ok_code)
		_exit(100 + (i->si_code & 0x7f));
	_exit(a >= (char *)target && a < (char *)target + target_len ? 0 : 3);
}

/* In a child, reads *at with sig caught: 0 right code and address; 1 no signal; 2 wrong code; 3 wrong address. */
static int fault(int sig, char *at, long len)
{
	pid_t kid;
	int st;

	target = at;
	target_len = len;
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		struct sigaction sa;
		memset(&sa, 0, sizeof sa);
		sa.sa_sigaction = hf;
		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		(void)sigaction(sig, &sa, NULL);
		(void)*target;
		_exit(1);
	}
	if (waitpid(kid, &st, 0) != kid || !WIFEXITED(st))
		return 9;
	if (WEXITSTATUS(st) >= 100) {
		last_code = WEXITSTATUS(st) - 100;
		return 2;
	}
	return WEXITSTATUS(st);
}

int main(void)
{
	pc_begin("XSH.2.4.3-signal-actions.020");
	{
		long pg = sysconf(_SC_PAGESIZE);
		char *p;
		int fd, r;

		pc_check(pg > 0, "sysconf(_SC_PAGESIZE) failed");
		fd = open("g", O_RDWR | O_CREAT | O_EXCL, 0600);
		pc_check(fd >= 0 && ftruncate(fd, pg) == 0, "could not make a one-page file");
		p = mmap(NULL, (size_t)pg, PROT_READ, MAP_SHARED, fd, 0);
		pc_check(p != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
		pc_check(munmap(p, (size_t)pg) == 0, "munmap failed");
		fd = open("f", O_RDWR | O_CREAT | O_EXCL, 0600);
		pc_check(fd >= 0 && ftruncate(fd, 2 * pg) == 0, "could not make a two-page file");
		pc_test();
		r = fault(SIGSEGV, p, pg);
		pc_check(r != 1, "reading the unmapped page did not raise SIGSEGV");
		pc_check(r != 2, "SIGSEGV for an unmapped page has si_code %d, expected SEGV_MAPERR (%d)", last_code, SEGV_MAPERR);
		pc_check(r != 3, "SIGSEGV's si_addr is not in the unmapped page");
		pc_check(r == 0, "the faulting child failed (%d)", r);
		p = mmap(NULL, (size_t)(2 * pg), PROT_READ, MAP_SHARED, fd, 0);
		pc_check(p != MAP_FAILED, "mmap of the file failed: %s", pc_errname(errno));
		pc_check(ftruncate(fd, pg) == 0, "ftruncate failed: %s", pc_errname(errno));
		r = fault(SIGBUS, p + pg, pg);
		pc_check(r != 1, "reading beyond the file's end did not raise SIGBUS");
		pc_check(r != 2, "SIGBUS has si_code %d, expected BUS_ADRERR (%d) or BUS_OBJERR (%d)", last_code, BUS_ADRERR,
			 BUS_OBJERR);
		pc_check(r != 3, "SIGBUS's si_addr is not in the page beyond the file's end");
		pc_check(r == 0, "the faulting child failed (%d)", r);
	}
	pc_pass();
}
