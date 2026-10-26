/*
 * sleep(1) does not return before a second has passed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sleep.002
 * MUTANT    sleep.early
 * READING   sleep.early_half
 * VARIANT   sleep.late
 * VARIANT   sleep.via_alarm
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
	long long t0, got, tol;
	unsigned r;

	pc_begin("XSH.sleep.002");
	tol = mono_res();
	pc_test();
	t0 = mono();
	r = sleep(1);
	got = mono() - t0;
	pc_check(r == 0, "sleep(1) with no signal returned %u", r);
	pc_check(got >= 1000 * MS - tol, "sleep(1) returned after %lld.%03lld ms", got / MS, (got % MS) / 1000LL);
	pc_pass();
}
