/*
 * _exit() runs no thread cleanup handlers or destructors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.011
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

static int report_fd = -1;
static pthread_key_t key;
static int ready_fd = -1;

static void cleanup(void *arg)
{
	(void)arg;
	(void)write(report_fd, "c", 1);
}

static void destructor(void *arg)
{
	(void)arg;
	(void)write(report_fd, "d", 1);
}

static void *worker(void *arg)
{
	(void)arg;
	(void)pthread_setspecific(key, &key);
	pthread_cleanup_push(cleanup, NULL);
	(void)write(ready_fd, "r", 1);
	for (;;)
		(void)sleep(1);
	pthread_cleanup_pop(0);
	return NULL;
}

/* Waits for pid and returns its raw status; anything odd is UNRESOLVED. */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	return st;
}

static pid_t spawn(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	return r;
}

int main(void)
{
	int p[2], q[2], st;
	pid_t c;
	char ch;

	pc_begin("XSH._Exit.011");
	pc_check(pipe(p) == 0 && pipe(q) == 0, "pipe failed");
	report_fd = p[1];
	pc_test();
	c = spawn();
	if (c == 0) {
		pthread_t th;
		char r;
		(void)close(p[0]);
		ready_fd = q[1];
		if (pthread_key_create(&key, destructor) != 0 ||
		    pthread_setspecific(key, &key) != 0 ||
		    pthread_create(&th, NULL, worker, NULL) != 0)
			_exit(99);
		/* The calling thread has a value and a cleanup handler too. */
		pthread_cleanup_push(cleanup, NULL);
		if (read(q[0], &r, 1) != 1)
			_exit(99);
		_exit(0);
		pthread_cleanup_pop(0);
	}
	(void)close(p[1]);
	(void)close(q[1]);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (%#x)", st);
	pc_check(read(p[0], &ch, 1) == 0, "a cleanup handler or destructor ran ('%c')", ch);
	pc_pass();
}
