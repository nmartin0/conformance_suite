/*
 * A pipe read returns what is there.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.010
 * MUTANT    read.waits_for_full
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

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
	char buf[16];
	ssize_t r;

	pc_begin("XSH.read.010");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(write(p[1], "abc", 3) == 3, "write failed");
	catch_alarm();
	start_timer(5);
	pc_test();
	r = read(p[0], buf, sizeof buf);
	pc_check(!fired, "read() waited for more data until the 5 s timer fired");
	pc_check(r >= 1 && r <= 3, "read(16) with 3 bytes in the pipe returned %ld (%s)",
	    (long)r, r < 0 ? pc_errname(errno) : "-");
	pc_check(memcmp(buf, "abc", (size_t)r) == 0, "read the wrong bytes");
	pc_pass();
}
