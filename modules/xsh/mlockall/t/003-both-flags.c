/*
 * mlockall(MCL_CURRENT|MCL_FUTURE) locks earlier and later mappings.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mlockall.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static inline void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out (a call did not return)");
}

/* Limit the whole test: a hang becomes a FAIL (cleanup runs at exit). */
static inline void guard(unsigned secs)
{
	(void)signal(SIGALRM, on_alarm);
	(void)alarm(secs);
}

static inline void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Run fn in a child; its exit status is what fn returns (0 to 96). */
static inline pid_t spawn(int (*fn)(void))
{
	pid_t k = fork();

	if (k == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (k == 0) {
		(void)signal(SIGALRM, SIG_DFL);
		(void)alarm(25);
		_exit(fn());
	}
	return k;
}

/* Exit status of k; -1 if it did not end within secs (it is killed), -2 if a signal ended it. */
static inline int reap(pid_t k, int secs)
{
	int st, i;

	for (i = 0; i < secs * 100; i++) {
		pid_t r = waitpid(k, &st, WNOHANG);

		if (r == k)
			return WIFEXITED(st) ? WEXITSTATUS(st) : -2;
		if (r == -1 && errno != EINTR)
			return -2;
		sleep_ms(10);
	}
	(void)kill(k, SIGKILL);
	(void)waitpid(k, &st, 0);
	return -1;
}

/* Whether k has not ended yet (its status stays collectable). */
static inline int running(pid_t k)
{
	siginfo_t si;

	si.si_pid = 0;
	if (waitid(P_PID, (id_t)k, &si, WEXITED | WNOHANG | WNOWAIT) != 0)
		pc_unresolved("waitid failed: %s", pc_errname(errno));
	return si.si_pid == 0;
}

/*
 * Child result for "the call must fail with err": 0 when it did, 1 when it
 * succeeded, 100 + errno for another error.
 */
static inline int want_err(long r, int err)
{
	if (r != -1)
		return 1;
	return errno == err ? 0 : 100 + (errno < 150 ? errno : 149);
}

static inline const char *child_said(int rc)
{
	static char buf[64];

	if (rc == 0)
		return "as expected";
	if (rc == 1)
		return "the call succeeded";
	if (rc >= 100) {
		snprintf(buf, sizeof buf, "failed with %s", pc_errname(rc - 100));
		return buf;
	}
	if (rc < 0)
		return "the child did not end normally or in time";
	snprintf(buf, sizeof buf, "code %d", rc);
	return buf;
}

/*
 * Run fn in a child that has no appropriate privileges: when the test runs
 * as root, the child takes user and group 65534. Returns what fn returns
 * (0 to 96); a child that cannot drop privileges makes the test UNRESOLVED.
 */
static inline int as_other(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (geteuid() == 0) {
			(void)pc_drop_groups();
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		(void)signal(SIGALRM, SIG_DFL);
		(void)alarm(25);
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

static inline void in_range(long v, long lo, long hi, const char *what)
{
	pc_check(v >= lo && v <= hi, "%s is %ld, expected between %ld and %ld", what, v, lo, hi);
}

/* A handler that does nothing, installed without SA_RESTART. */
static inline void noop_handler(int s)
{
	(void)s;
}

static inline void catch_usr1(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = noop_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, NULL) != 0)
		pc_unresolved("sigaction failed: %s", pc_errname(errno));
}

/* Send SIGUSR1 to k every 100 ms until it ends; its exit status as reap(). */
static inline int interrupt_until_done(pid_t k, int secs)
{
	int i, st;

	for (i = 0; i < secs * 10; i++) {
		pid_t r;

		sleep_ms(100);
		r = waitpid(k, &st, WNOHANG);
		if (r == k)
			return WIFEXITED(st) ? WEXITSTATUS(st) : -2;
		(void)kill(k, SIGUSR1);
	}
	(void)kill(k, SIGKILL);
	(void)waitpid(k, &st, 0);
	return -1;
}

#include <sys/mman.h>

static long pg;

static inline void get_page_size(void)
{
	pg = sysconf(_SC_PAGESIZE);
	if (pg <= 0)
		pc_unresolved("sysconf(_SC_PAGESIZE) gave %ld", pg);
}

/* A file of n pages; returns an O_RDWR descriptor (setup). */
static inline int make_pages(const char *path, long n)
{
	int fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0600);

	if (fd < 0)
		pc_unresolved("cannot create %s: %s", path, pc_errname(errno));
	if (ftruncate(fd, (off_t)(n * pg)) != 0)
		pc_unresolved("ftruncate failed: %s", pc_errname(errno));
	return fd;
}

/* A MAP_SHARED mapping of the whole of fd (setup). */
static inline char *map_file(int fd, long n)
{
	void *p = mmap(NULL, (size_t)(n * pg), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (p == MAP_FAILED)
		pc_unresolved("mmap failed: %s", pc_errname(errno));
	return p;
}

/*
 * Whether some page of [p, p+len) is locked, seen through msync() with
 * MS_INVALIDATE, which fails with EBUSY over locked pages (XSH.msync.004).
 */
static inline int locked(void *p, size_t len)
{
	int r = msync(p, len, MS_ASYNC | MS_INVALIDATE);

	if (r == -1 && errno == EBUSY)
		return 1;
	if (r != 0)
		pc_unresolved("msync(MS_ASYNC|MS_INVALIDATE) failed: %s", pc_errname(errno));
	return 0;
}

/* mlockall() failed for want of resources or privileges: not under test here. */
static inline void lock_or_skip(int r)
{
	if (r == -1 && (errno == EAGAIN || errno == ENOMEM || errno == EPERM))
		pc_unresolved("mlockall() failed: %s (resources or privileges)", pc_errname(errno));
}

int main(void)
{
	pc_begin("XSH.mlockall.003");

	int fd, r;
	char *p1, *p2;

	get_page_size();
	fd = make_pages("f", 2);
	p1 = map_file(fd, 2);
	pc_test();
	r = mlockall(MCL_CURRENT | MCL_FUTURE);
	lock_or_skip(r);
	pc_check(r == 0, "mlockall returned %d (%s)", r, pc_errname(errno));
	p2 = map_file(fd, 2);
	pc_check(locked(p1, (size_t)(2 * pg)), "the earlier mapping is not locked");
	pc_check(locked(p2, (size_t)(2 * pg)), "the later mapping is not locked");
	pc_check(munlockall() == 0, "munlockall failed: %s", pc_errname(errno));
	pc_check(!locked(p1, (size_t)(2 * pg)), "the earlier mapping is still locked after munlockall()");
	pc_check(!locked(p2, (size_t)(2 * pg)), "the later mapping is still locked after munlockall()");
	pc_pass();
}
