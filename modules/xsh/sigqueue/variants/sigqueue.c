/*
 * sigqueue.c - conforming variants of sigqueue(), for the variant checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

#define QMAX 32	/* {_POSIX_SIGQUEUE_MAX} */

/*
 * "max_min": {SIGQUEUE_MAX} is at its minimum, 32 (XBD <limits.h> gives
 * _POSIX_SIGQUEUE_MAX as the least value allowed), and sysconf() says so;
 * sigqueue() ERRORS then requires EAGAIN once this process has 32 queued
 * signals still pending at the receivers. The count kept here covers the
 * signals a process queues to itself with SA_SIGINFO set, and drops by one
 * each time a sigwait function accepts one of them (XSH sigtimedwait():
 * accepting releases the resource). Signals delivered to a handler, or
 * queued to another process, are not tracked, so the variant is declared
 * only by tests that queue to themselves and accept with sigtimedwait().
 */
static pid_t owner;
static long queued;

static void sync_owner(void)
{
	if (owner != getpid()) {
		owner = getpid();
		queued = 0;
	}
}

static int has_siginfo(int sig)
{
	struct sigaction sa;

	return sigaction(sig, NULL, &sa) == 0 && (sa.sa_flags & SA_SIGINFO) != 0;
}

static void accepted(const char *v, int r, const siginfo_t *si)
{
	if (IS("max_min") && r > 0 && si->si_code == SI_QUEUE && si->si_pid == getpid()) {
		sync_owner();
		if (queued > 0)
			queued--;
	}
}

long sysconf(int name)
{
	const char *v = variant_of("sigqueue.");
	REAL(sysconf);

	if (IS("max_min") && name == _SC_SIGQUEUE_MAX)
		return QMAX;
	return real_sysconf(name);
}

int sigqueue(pid_t pid, int sig, union sigval value)
{
	const char *v = variant_of("sigqueue.");
	int r, counted = 0;
	REAL(sigqueue);

	if (IS("max_min") && sig > 0 && pid == getpid() && has_siginfo(sig)) {
		sync_owner();
		if (queued >= QMAX) {
			errno = EAGAIN;
			return -1;
		}
		counted = 1;
	}
	/*
	 * "plain_once": without SA_SIGINFO for the signal, sigqueue()
	 * DESCRIPTION promises only that the signal is sent at least once
	 * and leaves it unspecified whether the value goes with it, and XSH
	 * 2.4.2 leaves it unspecified whether such signals are queued. Here
	 * they are not: a sigqueue() to the caller while the signal is
	 * already pending merges with the pending one (and succeeds), and
	 * the value sent is not the caller's.
	 */
	if (IS("plain_once") && sig > 0 && pid == getpid() && !has_siginfo(sig)) {
		sigset_t p;

		if (sigpending(&p) == 0 && sigismember(&p, sig) == 1)
			return 0;
		value.sival_int = -1;
	}
	r = real_sigqueue(pid, sig, value);
	if (r == 0 && counted)
		queued++;
	return r;
}

int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *to)
{
	const char *v = variant_of("sigqueue.");
	siginfo_t local;
	int r;
	REAL(sigtimedwait);

	if (info == NULL)
		info = &local;
	r = real_sigtimedwait(set, info, to);
	accepted(v, r, info);
	return r;
}

int sigwaitinfo(const sigset_t *set, siginfo_t *info)
{
	const char *v = variant_of("sigqueue.");
	siginfo_t local;
	int r;
	REAL(sigwaitinfo);

	if (info == NULL)
		info = &local;
	r = real_sigwaitinfo(set, info);
	accepted(v, r, info);
	return r;
}
