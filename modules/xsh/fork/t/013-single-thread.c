/*
 * The child of a threaded process has one thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.013
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

static volatile unsigned long ticks;

static void *counter(void *arg)
{
	(void)arg;
	for (;;)
		ticks++;
	return NULL;
}

/*
 * Waits for the child; returns its exit status (0 = all its checks held).
 * A child that did not exit normally is UNRESOLVED.
 */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (!WIFEXITED(st))
		pc_unresolved("the child was killed by signal %d", WIFSIGNALED(st) ? WTERMSIG(st) : 0);
	return WEXITSTATUS(st);
}

int main(void)
{
	pthread_t th;
	pid_t r;
	int code;

	pc_begin("XSH.fork.013");
	pc_check(pthread_create(&th, NULL, counter, NULL) == 0, "pthread_create failed");
	while (ticks < 1000)
		;	/* the counting thread is running */
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		unsigned long before = ticks;
		struct timespec ts = { 0, 200000000 };
		(void)nanosleep(&ts, NULL);
		_exit(ticks == before ? 0 : 1);
	}
	code = reap(r);
	pc_check(code == 0, "a second thread runs in the child");
	pc_pass();
}
