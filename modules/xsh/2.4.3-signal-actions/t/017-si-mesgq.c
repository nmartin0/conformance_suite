/*
 * A message queue notification has si_code SI_MESGQ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.4.3-signal-actions.017
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <mqueue.h>

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero. */
static int wait_for(volatile sig_atomic_t *flag, long ms)
{
	long i;

	for (i = 0; i < ms / 10 && !*flag; i++)
		sleep_ms(10);
	return *flag != 0;
}

static long ms_since(const struct timespec *t0)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long)(t.tv_sec - t0->tv_sec) * 1000 + (t.tv_nsec - t0->tv_nsec) / 1000000;
}

static volatile sig_atomic_t got;
static volatile int code, val, signo;

static void h(int s, siginfo_t *i, void *u)
{
	(void)u;
	signo = s;
	code = i->si_code;
	val = i->si_value.sival_int;
	got = 1;
}

static void catch_info(int sig)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = h;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction failed");
}

int main(void)
{
	pc_begin("XSH.2.4.3-signal-actions.017");
	{
		struct mq_attr at;
		struct sigevent ev;
		char name[64];
		mqd_t q;

		catch_info(SIGRTMIN);
		snprintf(name, sizeof name, "/pc-mq-%ld", (long)getpid());
		(void)mq_unlink(name);
		memset(&at, 0, sizeof at);
		at.mq_maxmsg = 4;
		at.mq_msgsize = 16;
		q = mq_open(name, O_RDWR | O_CREAT | O_EXCL, 0600, &at);
		pc_check(q != (mqd_t)-1, "mq_open failed: %s", pc_errname(errno));
		(void)mq_unlink(name);
		memset(&ev, 0, sizeof ev);
		ev.sigev_notify = SIGEV_SIGNAL;
		ev.sigev_signo = SIGRTMIN;
		ev.sigev_value.sival_int = 77;
		pc_test();
		pc_check(mq_notify(q, &ev) == 0, "mq_notify failed: %s", pc_errname(errno));
		pc_check(mq_send(q, "x", 1, 0) == 0, "mq_send failed: %s", pc_errname(errno));
		pc_check(wait_for(&got, 3000), "the handler did not run");
		pc_check(code == SI_MESGQ, "si_code is %d, expected SI_MESGQ (%d)", code, SI_MESGQ);
		pc_check(val == 77, "si_value is %d, expected 77", val);
	}
	pc_pass();
}
