/*
 * An interrupted blocking read gives EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.011
 * MUTANT    read.eintr_as_eagain
 * MUTANT    read.eintr_restarts
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

static volatile sig_atomic_t fired;
static unsigned period;
/*
 * After two alarms the handler unblocks a call that wrongly restarted
 * instead of failing with EINTR, so the test can FAIL rather than time
 * out: it writes to rescue_wfd, reads from rescue_rfd, or closes
 * rescue_cfd (all async-signal-safe).
 */
static int rescue_wfd = -1, rescue_rfd = -1, rescue_cfd = -1;

static void on_alarm(int sig)
{
	static char junk[65536];
	(void)sig;
	fired++;
	if (fired >= 2) {
		if (rescue_wfd >= 0)
			(void)write(rescue_wfd, junk, 64);
		if (rescue_rfd >= 0)
			(void)read(rescue_rfd, junk, sizeof junk);
		if (rescue_cfd >= 0) {
			(void)close(rescue_cfd);
			rescue_cfd = -1;
		}
	}
	alarm(period);
}

/* A caught SIGALRM (no SA_RESTART), unblocked, that interrupts blocked calls. */
static void catch_alarm(void)
{
	struct sigaction sa;
	sigset_t s;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	sigemptyset(&s);
	sigaddset(&s, SIGALRM);
	pc_check(sigprocmask(SIG_UNBLOCK, &s, NULL) == 0, "cannot unblock SIGALRM");
}

/* SIGALRM every secs seconds (alarm() re-armed by the handler). */
static void start_timer(unsigned secs)
{
	period = secs;
	alarm(secs);
}

int main(void)
{
	int p[2];
	char c;

	pc_begin("XSH.read.011");
	pc_check(pipe(p) == 0, "pipe failed");
	catch_alarm();
	rescue_wfd = p[1];
	start_timer(1);
	pc_test();
	pc_expect_err(read(p[0], &c, 1), EINTR, "read() of an empty pipe interrupted by SIGALRM");
	pc_pass();
}
