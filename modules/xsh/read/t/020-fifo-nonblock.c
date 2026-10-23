/*
 * A non-blocking read of an empty FIFO gives EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.020
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
#include <sys/stat.h>

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
	int r, w;
	char c;

	pc_begin("XSH.read.020");
	pc_check(mkfifo("q", 0600) == 0, "mkfifo failed");
	r = open("q", O_RDONLY | O_NONBLOCK);
	w = open("q", O_WRONLY);
	pc_check(r >= 0 && w >= 0, "cannot open the FIFO");
	catch_alarm();
	start_timer(5);
	pc_test();
	pc_expect_err(read(r, &c, 1), EAGAIN, "non-blocking read() of an empty FIFO with a writer");
	pc_pass();
}
