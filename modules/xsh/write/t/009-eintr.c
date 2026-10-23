/*
 * An interrupted write that wrote nothing gives EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.009
 * MUTANT    write.eintr_as_eagain
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
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

int main(void)
{
	int p[2], fl;
	char c = 'x';

	pc_begin("XSH.write.009");
	pc_check(pipe(p) == 0, "pipe failed");
	(void)fill(p[1]);
	fl = fcntl(p[1], F_GETFL);
	pc_check(fl != -1 && fcntl(p[1], F_SETFL, fl & ~O_NONBLOCK) == 0, "cannot clear O_NONBLOCK");
	catch_alarm();
	pc_test();
	start_timer(1);
	pc_expect_err(write(p[1], &c, 1), EINTR, "write() to a full pipe interrupted by SIGALRM");
	pc_pass();
}
