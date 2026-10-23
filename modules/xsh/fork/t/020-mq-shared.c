/*
 * The child's message queue descriptor shares the parent's description.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.020
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
	pc_begin("XSH.fork.020");
	{
		struct mq_attr at, now;
		char name[64];
		mqd_t q;
		pid_t kid;
		int st;

		snprintf(name, sizeof name, "/pc-mqf-%ld", (long)getpid());
		(void)mq_unlink(name);
		memset(&at, 0, sizeof at);
		at.mq_maxmsg = 4;
		at.mq_msgsize = 16;
		q = mq_open(name, O_RDWR | O_CREAT | O_EXCL, 0600, &at);
		pc_check(q != (mqd_t)-1, "mq_open failed: %s", pc_errname(errno));
		(void)mq_unlink(name);
		pc_check(mq_send(q, "hi", 2, 0) == 0, "mq_send failed");
		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0) {
			char buf[16];
			struct mq_attr nb;
			if (mq_receive(q, buf, sizeof buf, NULL) != 2 || memcmp(buf, "hi", 2) != 0)
				_exit(10);
			memset(&nb, 0, sizeof nb);
			nb.mq_flags = O_NONBLOCK;
			_exit(mq_setattr(q, &nb, NULL) == 0 ? 0 : 11);
		}
		pc_check(waitpid(kid, &st, 0) == kid && WIFEXITED(st), "the child did not exit");
		pc_check(WEXITSTATUS(st) != 10, "the child could not receive through the inherited descriptor");
		pc_check(WEXITSTATUS(st) == 0, "mq_setattr() failed in the child");
		pc_check(mq_getattr(q, &now) == 0, "mq_getattr failed");
		pc_check(now.mq_flags & O_NONBLOCK, "O_NONBLOCK set by the child is not seen by the parent");
	}
	pc_pass();
}
