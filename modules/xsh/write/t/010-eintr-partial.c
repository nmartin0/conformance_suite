/*
 * An interrupted write that wrote some data returns the count.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.010
 * MUTANT    write.partial_as_eintr
 * MUTANT    write.eintr_restarts
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

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
	char *buf;
	ssize_t r;

	pc_begin("XSH.write.010");
	pc_check(pipe(p) == 0, "pipe failed");
	buf = calloc(1, 1 << 20);
	pc_check(buf != NULL, "out of memory");
	catch_alarm();
	rescue_rfd = p[0];
	pc_test();
	start_timer(1);
	r = write(p[1], buf, 1 << 20);
	if (r == (1 << 20) && !fired)
		pc_unresolved("a 1 MiB write to the pipe completed without blocking");
	if (r == -1 && errno == EINTR) {
		char ch;
		int fl = fcntl(p[0], F_GETFL);

		if (fl != -1 && fcntl(p[0], F_SETFL, fl | O_NONBLOCK) == 0 && read(p[0], &ch, 1) == -1 && errno == EAGAIN)
			pc_unresolved("nothing was written before the signal, so EINTR is the right result");
	}
	pc_check(r > 0 && r < (1 << 20), "the interrupted write returned %ld (%s), expected a partial count",
	    (long)r, r < 0 ? pc_errname(errno) : "-");
	pc_pass();
}
