/*
 * A null handler address is accepted and only the other handlers run.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_atfork.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static volatile int pre_runs, par_runs;
static int wr = -1;

static void pre(void) { pre_runs++; }
static void par(void) { par_runs++; }
static void ch(void) { char c = 'c'; (void)write(wr, &c, 1); }

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	int fds[2], r, st;
	pid_t pid;
	char buf[8];
	ssize_t n, got = 0;

	pc_begin("XSH.pthread_atfork.002");
	NEED(pipe(fds) == 0, "pipe failed: %s", pc_errname(errno));
	wr = fds[1];
	pc_test();
	r = pthread_atfork(pre, NULL, NULL);
	pc_check(r == 0, "pthread_atfork(prepare, NULL, NULL) returned %s", rn(r));
	r = pthread_atfork(NULL, par, NULL);
	pc_check(r == 0, "pthread_atfork(NULL, parent, NULL) returned %s", rn(r));
	r = pthread_atfork(NULL, NULL, ch);
	pc_check(r == 0, "pthread_atfork(NULL, NULL, child) returned %s", rn(r));
	pid = fork();
	NEED(pid != -1, "fork failed: %s", pc_errname(errno));
	if (pid == 0)
		_exit(0);
	while (waitpid(pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	(void)close(fds[1]);
	while ((n = read(fds[0], buf + got, sizeof buf - (size_t)got)) > 0)
		got += n;
	pc_check(pre_runs == 1, "the prepare handler ran %d times, expected once", pre_runs);
	pc_check(par_runs == 1, "the parent handler ran %d times, expected once", par_runs);
	pc_check(got == 1 && buf[0] == 'c', "the child handler wrote %zd bytes, expected one", got);
	pc_pass();
}
