/*
 * A large non-blocking pipe write moves at least {PIPE_BUF} bytes, or gives EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.014
 * MUTANT    write.pipe_small_transfer
 * MUTANT    write.nonblock_blocks
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>

static volatile sig_atomic_t fired;

static void on_alarm(int sig)
{
	(void)sig;
	fired = 1;
}

/* A caught, repeating SIGALRM (no SA_RESTART) that interrupts blocked calls. */
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

static void start_timer(unsigned secs)
{
	struct itimerval it;

	it.it_interval.tv_sec = secs;
	it.it_interval.tv_usec = 0;
	it.it_value = it.it_interval;
	pc_check(setitimer(ITIMER_REAL, &it, NULL) == 0, "setitimer failed");
}

int main(void)
{
	int p[2], fl;
	char *buf;
	ssize_t r;
	size_t n = 1 << 20;

	pc_begin("XSH.write.014");
	pc_check(pipe(p) == 0, "pipe failed");
	fl = fcntl(p[1], F_GETFL);
	pc_check(fl != -1 && fcntl(p[1], F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	buf = calloc(1, n);
	pc_check(buf != NULL, "out of memory");
	catch_alarm();
	start_timer(5);
	pc_test();
	r = write(p[1], buf, n);
	pc_check(r >= PIPE_BUF && (size_t)r <= n, "write of 1 MiB to an empty pipe returned %ld", (long)r);
	while ((r = write(p[1], buf, n)) > 0)
		;	/* now fill it */
	pc_expect_err(r, EAGAIN, "non-blocking write of more than {PIPE_BUF} bytes to a full pipe");
	pc_pass();
}
