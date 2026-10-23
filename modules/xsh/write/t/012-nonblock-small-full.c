/*
 * A small non-blocking pipe write is all or nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.012
 * MUTANT    write.nonblock_partial_small
 * MUTANT    write.nonblock_blocks
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

/* Fill the pipe completely (setup): non-blocking writes until EAGAIN. */
static long fill(int w)
{
	static char big[4096];
	long total = 0;
	int fl = fcntl(w, F_GETFL);
	ssize_t r;

	pc_check(fl != -1 && fcntl(w, F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	while ((r = write(w, big, sizeof big)) > 0)
		total += r;
	while ((r = write(w, big, 1)) > 0)
		total += r;
	pc_check(r == -1 && errno == EAGAIN, "could not fill the pipe");
	return total;
}

/* {PIPE_BUF}: from <limits.h>, else from fpathconf() on the pipe. */
static long pipe_buf(int fd)
{
#ifdef PIPE_BUF
	(void)fd;
	return PIPE_BUF;
#else
	long v = fpathconf(fd, _PC_PIPE_BUF);
	if (v <= 0)
		pc_unresolved("{PIPE_BUF} is indeterminate");
	return v;
#endif
}

int main(void)
{
	long pb;
	int p[2], fl;
	long total, after = 0;
	char *buf;
	ssize_t r;

	pc_begin("XSH.write.012");
	pc_check(pipe(p) == 0, "pipe failed");
	pb = pipe_buf(p[1]);
	total = fill(p[1]);
	buf = calloc(1, (size_t)total + pb);
	pc_check(buf != NULL, "out of memory");
	pc_check(read(p[0], buf, 10) == 10, "cannot read 10 bytes out");
	fl = fcntl(p[0], F_GETFL);
	pc_check(fl != -1 && fcntl(p[0], F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	catch_alarm();
	start_timer(5);
	pc_test();
	r = write(p[1], buf, pb);
	if (r == pb) {
		/* There was room after all: nothing to judge. */
		pc_unresolved("the pipe had room for {PIPE_BUF} bytes after 10 were read");
	}
	pc_expect_err(r, EAGAIN, "non-blocking write of {PIPE_BUF} bytes without room");
	while ((r = read(p[0], buf, (size_t)total)) > 0)
		after += r;
	pc_check(after == total - 10, "%ld bytes in the pipe, expected %ld: data was transferred",
	    after, total - 10);
	pc_pass();
}
