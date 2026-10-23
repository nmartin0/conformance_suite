/*
 * A non-blocking read of an empty socket gives EAGAIN or EWOULDBLOCK.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.019
 * MUTANT    read.nonblock_blocks
 * MUTANT    read.eagain_as_eintr
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/socket.h>

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
	int sv[2], fl, e;
	char c;
	ssize_t r;

	pc_begin("XSH.read.019");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	fl = fcntl(sv[0], F_GETFL);
	pc_check(fl != -1 && fcntl(sv[0], F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	catch_alarm();
	start_timer(5);
	pc_test();
	r = read(sv[0], &c, 1);
	e = errno;
	pc_check(r == -1 && (e == EAGAIN || e == EWOULDBLOCK),
	    "non-blocking read() of an empty socket gave %ld (%s)", (long)r,
	    r < 0 ? pc_errname(e) : "-");
	pc_pass();
}
