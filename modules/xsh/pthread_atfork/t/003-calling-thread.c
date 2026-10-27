/*
 * Fork handlers run in the thread that called fork().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_atfork.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static pthread_t forker;
static volatile int pre_ok = -1, par_ok = -1;
static int wr = -1;
static pid_t result_pid = -1;

static void pre(void) { pre_ok = pthread_equal(pthread_self(), forker) != 0; }
static void par(void) { par_ok = pthread_equal(pthread_self(), forker) != 0; }
static void ch(void) { char c = 'y'; (void)write(wr, &c, 1); }

static void *thread_main(void *arg)
{
	(void)arg;
	forker = pthread_self();
	result_pid = fork();
	if (result_pid == 0)
		_exit(0);
	return NULL;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	int fds[2], r, st;
	pthread_t t;
	char c = '?';
	ssize_t n;

	pc_begin("XSH.pthread_atfork.003");
	NEED(pipe(fds) == 0, "pipe failed: %s", pc_errname(errno));
	wr = fds[1];
	pc_test();
	r = pthread_atfork(pre, par, ch);
	pc_check(r == 0, "pthread_atfork() returned %s", rn(r));
	NEED(pthread_create(&t, NULL, thread_main, NULL) == 0, "pthread_create failed");
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	NEED(result_pid > 0, "fork failed");
	while (waitpid(result_pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	(void)close(fds[1]);
	n = read(fds[0], &c, 1);
	pc_check(pre_ok == 1, "the prepare handler did not run in the thread that called fork()");
	pc_check(par_ok == 1, "the parent handler did not run in the thread that called fork()");
	pc_check(n == 1 && c == 'y', "the child handler did not run in the child (read %zd, '%c')", n, c);
	pc_pass();
}
