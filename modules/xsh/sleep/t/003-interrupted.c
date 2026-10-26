/*
 * A caught signal ends sleep() early, and it returns the unslept seconds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sleep.003
 * MUTANT    sleep.restarts
 * MUTANT    sleep.returns_slept
 * MUTANT    sleep.returns_zero
 * VARIANT   sleep.via_alarm
 * VARIANT   sleep.round_nearest
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

int main(void)
{
	long long t0, got, tol, low;
	unsigned r;
	pid_t c;

	pc_begin("XSH.sleep.003");
	catch_sig(SIGUSR1);
	tol = mono_res();
	pc_test();
	t0 = mono();
	c = send_repeat(SIGUSR1, 1000, 1000);
	r = sleep(10);
	got = mono() - t0;
	stop_sender(c);
	NEED(caught > 0, "no SIGUSR1 was caught");
	pc_check(got < 9000 * MS, "sleep(10) went on for %lld ms although SIGUSR1 was caught from 1 s on", got / MS);
	/* unslept = 10 s - slept >= 10 s - got; rounded down at worst */
	low = (10000 * MS - got - tol) / (1000 * MS);
	pc_check((long long)r >= low && r <= 10, "sleep(10) ended after %lld ms returned %u, expected %lld to 10",
	    got / MS, r, low);
	pc_pass();
}
