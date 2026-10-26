/*
 * An absolute clock_nanosleep() to a time already reached returns at once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_nanosleep.005
 * MUTANT    clock_nanosleep.past_sleeps
 * MUTANT    clock_nanosleep.past_einval
 * VARIANT   clock_getres.coarse
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

static void past(clockid_t id, const char *name, long long back)
{
	struct timespec target, rem;
	long long goal = ns_of(id) - back, t0, took;
	int r;

	target.tv_sec = (time_t)(goal / 1000000000LL);
	target.tv_nsec = (long)(goal % 1000000000LL);
	t0 = ns_of(CLOCK_MONOTONIC);
	r = clock_nanosleep(id, TIMER_ABSTIME, &target, &rem);
	took = ns_of(CLOCK_MONOTONIC) - t0;
	pc_check(r == 0, "clock_nanosleep(%s, TIMER_ABSTIME) to a time %lld ms ago returned %d (%s)", name, back / MS,
	    r, pc_errname(r));
	pc_check(took < 500 * MS, "clock_nanosleep(%s, TIMER_ABSTIME) to a time %lld ms ago took %lld ms", name, back / MS,
	    took / MS);
}

int main(void)
{
	pc_begin("XSH.clock_nanosleep.005");
	pc_test();
	past(CLOCK_MONOTONIC, "CLOCK_MONOTONIC", 1000 * MS);
	past(CLOCK_MONOTONIC, "CLOCK_MONOTONIC", 0);
	past(CLOCK_REALTIME, "CLOCK_REALTIME", 1000 * MS);
	past(CLOCK_REALTIME, "CLOCK_REALTIME", 0);
	pc_pass();
}
