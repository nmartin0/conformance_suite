/*
 * A blocked or an ignored signal does not end sleep().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sleep.004
 * MUTANT    sleep.blocked_wakes
 * MUTANT    sleep.ignored_wakes
 * VARIANT   sleep.late
 * VARIANT   sleep.via_alarm
 * VARIANT   sleep.early_half
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

static long long mono(void)
{
	struct timespec t;

	if (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
		pc_unresolved("clock_gettime(CLOCK_MONOTONIC) failed: %s", pc_errname(errno));
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long mono_res(void)
{
	struct timespec r;

	if (clock_getres(CLOCK_MONOTONIC, &r) != 0)
		pc_unresolved("clock_getres(CLOCK_MONOTONIC) failed: %s", pc_errname(errno));
	return (long long)r.tv_sec * 1000000000LL + r.tv_nsec;
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
 * every_ms, until stop_sender() kills it; it waits with poll().
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

static void sleep_through(int sig, const char *how)
{
	long long t0, got, tol = mono_res();
	unsigned r;
	pid_t c;

	t0 = mono();
	c = send_repeat(sig, 100, 100);
	r = sleep(2);
	got = mono() - t0;
	stop_sender(c);
	pc_check(r == 0, "with the signal %s, sleep(2) returned %u after %lld ms", how, r, got / MS);
	pc_check(got >= 1000 * MS - tol, "with the signal %s, sleep(2) returned after %lld ms", how, got / MS);
}

int main(void)
{
	sigset_t set, pend;
	struct sigaction ign;

	pc_begin("XSH.sleep.004");
	catch_sig(SIGUSR1);
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	NEED(sigprocmask(SIG_BLOCK, &set, NULL) == 0, "sigprocmask failed");
	memset(&ign, 0, sizeof ign);
	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	NEED(sigaction(SIGUSR2, &ign, NULL) == 0, "sigaction(SIGUSR2) failed");
	pc_test();
	sleep_through(SIGUSR1, "blocked");
	NEED(sigpending(&pend) == 0, "sigpending failed");
	pc_check(sigismember(&pend, SIGUSR1) == 1, "the blocked SIGUSR1 is no longer pending after sleep()");
	pc_check(caught == 0, "the blocked SIGUSR1 was delivered");
	sleep_through(SIGUSR2, "ignored");
	pc_pass();
}
