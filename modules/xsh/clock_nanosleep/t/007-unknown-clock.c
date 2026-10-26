/*
 * clock_nanosleep() on a clock ID that is not a known clock: EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_nanosleep.007
 * MUTANT    clock_nanosleep.unknown_enotsup
 * MUTANT    clock_nanosleep.unknown_ok
 * VARIANT   clock_getres.extra_clock
 */
#include "pc.h"
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define MS 1000000LL

static long long ns_of(clockid_t id)
{
	struct timespec t;

	if (clock_gettime(id, &t) != 0)
		pc_unresolved("clock_gettime(%d) failed: %s", (int)id, pc_errname(errno));
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long res_of(clockid_t id)
{
	struct timespec r;

	if (clock_getres(id, &r) != 0)
		pc_unresolved("clock_getres(%d) failed: %s", (int)id, pc_errname(errno));
	return (long long)r.tv_sec * 1000000000LL + r.tv_nsec;
}

/*
 * An interval measured with CLOCK_REALTIME (the clock slept on)
 * and checked against CLOCK_MONOTONIC: when the two disagree, the realtime
 * clock was stepped or slewed meanwhile and the test cannot judge.
 */
struct span { long long r0, m0, tol; };

static void span_start(struct span *s)
{
	long long a = res_of(CLOCK_REALTIME), b = res_of(CLOCK_MONOTONIC);

	s->tol = a > b ? a : b;
	if (s->tol < 1)
		s->tol = 1;
	s->m0 = ns_of(CLOCK_MONOTONIC);
	s->r0 = ns_of(CLOCK_REALTIME);
}

/* The realtime interval since span_start(), in ns. */
static long long span_end(struct span *s)
{
	long long r = ns_of(CLOCK_REALTIME) - s->r0, m = ns_of(CLOCK_MONOTONIC) - s->m0, d = r - m;

	if (d < 0)
		d = -d;
	if (d > 2 * s->tol + MS)
		pc_unresolved("CLOCK_REALTIME and CLOCK_MONOTONIC disagree by %lld us over the interval (the clock was adjusted)",
		    d / 1000LL);
	return r;
}

static volatile sig_atomic_t caught;

static void on_sig(int sig)
{
	(void)sig;
	caught++;
}

static void catch_sig(int sig)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_sig;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/*
 * A child that sends sig to this process after first_ms and then every
 * every_ms, until stop_sender() kills it. It waits with poll(), so that no
 * fault planted in a sleep function reaches it.
 */
static pid_t send_repeat(int sig, int first_ms, int every_ms)
{
	pid_t parent = getpid(), c = fork();

	NEED(c != -1, "fork failed: %s", pc_errname(errno));
	if (c == 0) {
		(void)poll(NULL, 0, first_ms);
		for (;;) {
			if (kill(parent, sig) != 0)
				_exit(0);
			(void)poll(NULL, 0, every_ms);
		}
	}
	return c;
}

static void stop_sender(pid_t c)
{
	(void)kill(c, SIGKILL);
	while (waitpid(c, NULL, 0) == -1 && errno == EINTR)
		;
}

/* As in XSH.clock_getres.004. */
static const long candidates[] = { 0x7ffffff0L, 0x3ffffff7L, 0x12345679L, 0x5aa5a55aL };

int main(void)
{
	struct timespec rq, rem, ts;
	size_t i;
	int r;

	pc_begin("XSH.clock_nanosleep.007");
	rq.tv_sec = 0;
	rq.tv_nsec = 10000000L;
	pc_test();
	for (i = 0; i < sizeof candidates / sizeof candidates[0]; i++) {
		clockid_t id = (clockid_t)candidates[i];

		errno = 0;
		if (clock_gettime(id, &ts) == 0) {
			pc_note("clock ID %#lx is a known clock here; trying the next", candidates[i]);
			continue;
		}
		NEED(errno == EINVAL, "clock_gettime(%#lx) failed with %s, not EINVAL", candidates[i], pc_errname(errno));
		r = clock_nanosleep(id, 0, &rq, &rem);
		pc_check(r == EINVAL, "relative clock_nanosleep(%#lx) returned %d (%s), expected EINVAL", candidates[i], r,
		    r > 0 ? pc_errname(r) : "not an error number");
		r = clock_nanosleep(id, TIMER_ABSTIME, &rq, &rem);
		pc_check(r == EINVAL, "absolute clock_nanosleep(%#lx) returned %d (%s), expected EINVAL", candidates[i], r,
		    r > 0 ? pc_errname(r) : "not an error number");
		pc_pass();
	}
	pc_unresolved("every candidate clock ID names a known clock here");
}
