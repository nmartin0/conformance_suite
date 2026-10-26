/*
 * nanosleep() leaves signal actions and the signal mask alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nanosleep.007
 * MUTANT    nanosleep.alarm_action_left
 * MUTANT    nanosleep.alarm_blocked_left
 * VARIANT   nanosleep.late
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
 * An interval measured with CLOCK_REALTIME (the clock nanosleep() names)
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

static const int sigs[] = { SIGALRM, SIGUSR1, SIGUSR2 };

static void on_other(int sig)
{
	(void)sig;
}

static void snapshot(sigset_t *mask, struct sigaction *acts)
{
	size_t i;

	pc_check(sigprocmask(SIG_SETMASK, NULL, mask) == 0, "sigprocmask failed");
	for (i = 0; i < sizeof sigs / sizeof sigs[0]; i++)
		pc_check(sigaction(sigs[i], NULL, &acts[i]) == 0, "sigaction(%d) failed", sigs[i]);
}

static void compare(const char *when, const sigset_t *m0, const struct sigaction *a0)
{
	sigset_t m1;
	struct sigaction a1[3];
	size_t i;
	int s;

	snapshot(&m1, a1);
	for (s = 1; s < 32; s++)
		pc_check(sigismember(m0, s) == sigismember(&m1, s), "after %s nanosleep(), signal %d is %s blocked",
		    when, s, sigismember(&m1, s) == 1 ? "now" : "no longer");
	for (i = 0; i < sizeof sigs / sizeof sigs[0]; i++)
		pc_check(a0[i].sa_handler == a1[i].sa_handler, "after %s nanosleep(), the action of signal %d changed",
		    when, sigs[i]);
}

int main(void)
{
	struct timespec rq, rem;
	struct sigaction sa, a0[3];
	sigset_t set, m0;
	pid_t c;
	int r;

	pc_begin("XSH.nanosleep.007");
	catch_sig(SIGUSR1);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_other;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction(SIGALRM) failed");
	sigemptyset(&set);
	sigaddset(&set, SIGUSR2);
	NEED(sigprocmask(SIG_BLOCK, &set, NULL) == 0, "sigprocmask failed");
	pc_test();
	snapshot(&m0, a0);
	rq.tv_sec = 0;
	rq.tv_nsec = 100000000L;
	r = nanosleep(&rq, &rem);
	pc_check(r == 0, "nanosleep(100 ms) returned %d (%s)", r, pc_errname(errno));
	compare("an uninterrupted", &m0, a0);
	rq.tv_sec = 5;
	rq.tv_nsec = 0;
	c = send_repeat(SIGUSR1, 200, 300);
	r = nanosleep(&rq, &rem);
	stop_sender(c);
	pc_check(r == -1 && errno == EINTR, "nanosleep(5 s) with SIGUSR1 caught returned %d (%s)", r, pc_errname(errno));
	compare("an interrupted", &m0, a0);
	pc_pass();
}
