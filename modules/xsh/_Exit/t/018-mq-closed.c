/*
 * _exit() closes message queue descriptors: a notification registration goes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.018
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

int main(void)
{
	pc_begin("XSH._Exit.018");
	{
		struct mq_attr at;
		struct sigevent ev;
		char name[64];
		mqd_t q;
		pid_t kid;
		int st = 0, r;

		snprintf(name, sizeof name, "/pc-mqx-%ld", (long)getpid());
		(void)mq_unlink(name);
		memset(&at, 0, sizeof at);
		at.mq_maxmsg = 4;
		at.mq_msgsize = 16;
		q = mq_open(name, O_RDWR | O_CREAT | O_EXCL, 0600, &at);
		pc_check(q != (mqd_t)-1, "mq_open failed: %s", pc_errname(errno));
		(void)mq_unlink(name);
		memset(&ev, 0, sizeof ev);
		ev.sigev_notify = SIGEV_NONE;
		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0)
			_exit(mq_notify(q, &ev) == 0 ? 0 : 99);
		(void)waitpid(kid, &st, 0);
		if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
			pc_unresolved("the child could not register for notification");
		r = mq_notify(q, &ev);
		pc_check(r == 0, "mq_notify() after the child's exit failed: %s", pc_errname(errno));
	}
	pc_pass();
}
