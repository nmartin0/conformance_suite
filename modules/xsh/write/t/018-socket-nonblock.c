/*
 * A non-blocking write to a full socket gives EAGAIN or EWOULDBLOCK.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.018
 * MUTANT    write.nonblock_blocks
 * MUTANT    write.eagain_as_eintr
 * VARIANT   write.ewouldblock
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>

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
	static char buf[4096];
	int sv[2], fl, e, i;
	ssize_t r = 0;

	pc_begin("XSH.write.018");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	fl = fcntl(sv[0], F_GETFL);
	pc_check(fl != -1 && fcntl(sv[0], F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	catch_alarm();
	rescue_rfd = sv[1];
	start_timer(5);
	pc_test();
	for (i = 0; i < 100000 && (r = write(sv[0], buf, sizeof buf)) > 0; i++)
		;
	e = errno;
	pc_check(r == -1 && (e == EAGAIN || e == EWOULDBLOCK),
	    "non-blocking write to a full socket gave %ld (%s)", (long)r, r < 0 ? pc_errname(e) : "-");
	pc_pass();
}
