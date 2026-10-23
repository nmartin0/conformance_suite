/*
 * An interrupted write that wrote some data returns the count.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.010
 * MUTANT    write.partial_as_eintr
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
	int p[2];
	char *buf;
	ssize_t r;

	pc_begin("XSH.write.010");
	pc_check(pipe(p) == 0, "pipe failed");
	buf = calloc(1, 1 << 20);
	pc_check(buf != NULL, "out of memory");
	catch_alarm();
	pc_test();
	start_timer(1);
	r = write(p[1], buf, 1 << 20);
	pc_check(r > 0 && r < (1 << 20), "the interrupted write returned %ld (%s), expected a partial count",
	    (long)r, r < 0 ? pc_errname(errno) : "-");
	pc_pass();
}
