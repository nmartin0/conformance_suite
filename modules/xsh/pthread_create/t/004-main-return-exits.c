/*
 * Returning from main() acts as exit() with main's value, even while
 * another thread runs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.004
 * MUTANT    pthread_create.main_status_lost
 * MUTANT    pthread_create.main_skips_atexit
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

/* Sets *p to v and wakes every waiter. */
static void post(int *p, int v)
{
	(void)pthread_mutex_lock(&mx);
	*p = v;
	(void)pthread_cond_broadcast(&cv);
	(void)pthread_mutex_unlock(&mx);
}

/* Waits until *p is v. */
static void await(int *p, int v)
{
	(void)pthread_mutex_lock(&mx);
	while (*p != v)
		(void)pthread_cond_wait(&cv, &mx);
	(void)pthread_mutex_unlock(&mx);
}

/* The value of *p, read under the lock. */
static int peek(int *p)
{
	int v;

	(void)pthread_mutex_lock(&mx);
	v = *p;
	(void)pthread_mutex_unlock(&mx);
	return v;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static int out_fd = -1;
static int started;

static void at_exit_handler(void)
{
	(void)write(out_fd, "A", 1);
}

/* Blocks for good: the process must end without it. */
static void *blocker(void *arg)
{
	int never = 0;

	(void)arg;
	post(&started, 1);
	await(&never, 1);
	return NULL;
}

int main(void)
{
	int p[2], st;
	char buf[64];
	size_t n = 0;
	ssize_t r;
	pid_t pid;

	pc_begin("XSH.pthread_create.004");
	pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
	pc_test();
	pid = fork();
	if (pid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (pid == 0) {
		pthread_t t;
		static char sbuf[256];

		(void)close(p[0]);
		out_fd = p[1];
		if (dup2(p[1], 1) != 1 || setvbuf(stdout, sbuf, _IOFBF, sizeof sbuf) != 0)
			_exit(90);
		if (atexit(at_exit_handler) != 0)
			_exit(91);
		if (pthread_create(&t, NULL, blocker, NULL) != 0)
			_exit(92);
		await(&started, 1);
		(void)fputs("F", stdout);	/* stays in the buffer until exit() flushes it */
		return 7;
	}
	(void)close(p[1]);
	while (n < sizeof buf - 1 && (r = read(p[0], buf + n, sizeof buf - 1 - n)) != 0) {
		if (r < 0) {
			if (errno == EINTR)
				continue;
			pc_unresolved("read failed: %s", pc_errname(errno));
		}
		n += (size_t)r;
	}
	buf[n] = '\0';
	while (waitpid(pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (WIFEXITED(st) && WEXITSTATUS(st) >= 90 && WEXITSTATUS(st) <= 92)
		pc_unresolved("the child could not set up (status %d)", WEXITSTATUS(st));
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 7,
		 "the child's main() returned 7 but its status is %#x, expected exited with 7", (unsigned)st);
	pc_check(strchr(buf, 'A') != NULL, "the atexit() handler did not run (output \"%s\")", buf);
	pc_check(strchr(buf, 'F') != NULL, "buffered stdout was not flushed (output \"%s\")", buf);
	pc_pass();
}
