/*
 * A large non-blocking pipe write moves at least {PIPE_BUF} bytes, or gives EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.014
 * MUTANT    write.pipe_small_transfer
 * MUTANT    write.nonblock_blocks
 * VARIANT   write.nb_pipe_min
 * VARIANT   write.pipe_buf_512
 * VARIANT   write.pipe_buf_unset
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
	char *buf;
	ssize_t r;
	size_t n = 1 << 20;

	pc_begin("XSH.write.014");
	pc_check(pipe(p) == 0, "pipe failed");
	pb = pipe_buf(p[1]);
	fl = fcntl(p[1], F_GETFL);
	pc_check(fl != -1 && fcntl(p[1], F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	buf = calloc(1, n);
	pc_check(buf != NULL, "out of memory");
	catch_alarm();
	rescue_rfd = p[0];
	start_timer(5);
	pc_test();
	r = write(p[1], buf, n);
	pc_check(r >= pb && (size_t)r <= n, "write of 1 MiB to an empty pipe returned %ld", (long)r);
	while ((r = write(p[1], buf, n)) > 0)
		;	/* now fill it */
	pc_expect_err(r, EAGAIN, "non-blocking write of more than {PIPE_BUF} bytes to a full pipe");
	pc_pass();
}
