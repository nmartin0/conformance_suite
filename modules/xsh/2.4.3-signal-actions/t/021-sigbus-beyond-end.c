/*
 * A reference past a mapped file's end gives SIGBUS with si_addr.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.4.3-signal-actions.021
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

static long pg;
static int fd;
static char *target;
static int last_code;
static volatile sig_atomic_t check_code;

static void hf(int s, siginfo_t *i, void *u)
{
	char *a = i->si_addr;

	(void)u;
	if (check_code && s == SIGSEGV && i->si_code != SEGV_MAPERR) {
		char m[32];
		int n = snprintf(m, sizeof m, "%d", i->si_code);
		(void)write(3, m, (size_t)n);
		_exit(2);
	}
	_exit(a >= target && a < target + pg ? 0 : 3);
}

/*
 * In a child, with sig caught: maperr reads a page of the one-page file "g"
 * unmapped just before; else reads the second page of a two-page mapping of
 * "f" cut to one page. 0: right; 1: no signal; 2: wrong code; 3: wrong address.
 */
static int fault(int sig, int maperr)
{
	char buf[32];
	int p[2], st;
	pid_t kid;
	ssize_t n;

	pc_check(pipe(p) == 0, "pipe failed");
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		struct sigaction sa;
		char *m;
		(void)close(p[0]);
		if (dup2(p[1], 3) != 3)
			_exit(9);
		memset(&sa, 0, sizeof sa);
		sa.sa_sigaction = hf;
		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		(void)sigaction(sig, &sa, NULL);
		if (maperr) {
			m = mmap(NULL, (size_t)pg, PROT_READ, MAP_SHARED, fd, 0);
			if (m == MAP_FAILED || munmap(m, (size_t)pg) != 0)
				_exit(9);
			target = m;
			check_code = 1;
		} else {
			m = mmap(NULL, (size_t)(2 * pg), PROT_READ, MAP_SHARED, fd, 0);
			if (m == MAP_FAILED || ftruncate(fd, pg) != 0)
				_exit(9);
			target = m + pg;
		}
		(void)*(volatile char *)target;
		_exit(1);
	}
	(void)close(p[1]);
	n = read(p[0], buf, sizeof buf - 1);
	(void)close(p[0]);
	if (waitpid(kid, &st, 0) != kid || !WIFEXITED(st))
		return 9;
	if (WEXITSTATUS(st) == 2) {
		buf[n > 0 ? n : 0] = '\0';
		last_code = atoi(buf);
	}
	return WEXITSTATUS(st);
}

int main(void)
{
	pc_begin("XSH.2.4.3-signal-actions.021");
	{
		int r;

		pg = sysconf(_SC_PAGESIZE);
		pc_check(pg > 0, "sysconf(_SC_PAGESIZE) failed");
		fd = open("f", O_RDWR | O_CREAT | O_EXCL, 0600);
		pc_check(fd >= 0 && ftruncate(fd, 2 * pg) == 0, "could not make a two-page file");
		pc_test();
		r = fault(SIGBUS, 0);
		pc_check(r != 1, "reading beyond the file's end did not raise SIGBUS");
		pc_check(r != 3, "SIGBUS's si_addr is not in the page beyond the file's end");
		pc_check(r == 0, "the faulting child failed (%d)", r);
	}
	pc_pass();
}
