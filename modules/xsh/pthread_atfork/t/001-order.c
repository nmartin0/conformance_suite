/*
 * Fork handlers run in the order the standard fixes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_atfork.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static volatile int log_pre[4], npre, log_par[4], npar;
static int wr = -1;

static void pre_a(void) { log_pre[npre++] = 1; }
static void pre_b(void) { log_pre[npre++] = 2; }
static void par_a(void) { log_par[npar++] = (npre << 4) | 1; }
static void par_b(void) { log_par[npar++] = (npre << 4) | 2; }
static void ch_a(void) { char c = 'a'; (void)write(wr, &c, 1); }
static void ch_b(void) { char c = 'b'; (void)write(wr, &c, 1); }

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

	pc_begin("XSH.pthread_atfork.001");
	NEED(pipe(fds) == 0, "pipe failed: %s", pc_errname(errno));
	wr = fds[1];
	pc_test();
	r = pthread_atfork(pre_a, par_a, ch_a);
	pc_check(r == 0, "first pthread_atfork() returned %s", rn(r));
	r = pthread_atfork(pre_b, par_b, ch_b);
	pc_check(r == 0, "second pthread_atfork() returned %s", rn(r));
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
	pc_check(npre == 2, "%d prepare handlers ran, expected 2", npre);
	pc_check(log_pre[0] == 2 && log_pre[1] == 1, "the prepare handlers ran in the order %d, %d, expected B, A (2, 1)",
	    log_pre[0], log_pre[1]);
	pc_check(npar == 2, "%d parent handlers ran, expected 2", npar);
	pc_check(log_par[0] == ((2 << 4) | 1) && log_par[1] == ((2 << 4) | 2),
	    "the parent handlers ran in the order %#x, %#x, expected A, B after both prepare handlers", log_par[0], log_par[1]);
	pc_check(got == 2 && buf[0] == 'a' && buf[1] == 'b', "the child handlers wrote %zd bytes, first %c%c, expected \"ab\" (A, B)", got,
	    got > 0 ? buf[0] : '?', got > 1 ? buf[1] : '?');
	pc_pass();
}
