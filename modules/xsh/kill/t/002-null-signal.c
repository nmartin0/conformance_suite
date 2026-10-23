/*
 * kill() with signal 0 checks the process exists but sends nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill.002
 * MUTANT    kill.null_sends
 * MUTANT    kill.esrch_as_eperm
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Installs fn for sig (FAIL or UNRESOLVED by phase). */
static void catch_sig(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/* A set holding the listed signals (end the list with 0). */
static sigset_t set_of(int first, ...)
{
	sigset_t s;
	va_list ap;
	int sig;

	sigemptyset(&s);
	va_start(ap, first);
	for (sig = first; sig != 0; sig = va_arg(ap, int))
		sigaddset(&s, sig);
	va_end(ap);
	return s;
}

/*
 * A child that reports each SIGURG (or SIGUSR1) it gets as one byte on the
 * pipe, after telling the parent it is ready; it lives until killed.
 * group: 0 stays in our group, 1 makes a group of its own.
 */
static int kid_w = -1;

static void on_sig(int sig)
{
	char c = sig == SIGURG ? 'u' : '1';
	(void)write(kid_w, &c, 1);
}

static pid_t kid(int p[2], int group)
{
	int ready[2];
	pid_t c;
	char ch;

	if (pipe(ready) != 0)
		pc_unresolved("pipe failed");
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		kid_w = p[1];
		(void)close(p[0]);
		(void)close(ready[0]);
		if (group && setpgid(0, 0) != 0)
			_exit(99);
		catch_sig(SIGURG, on_sig);
		catch_sig(SIGUSR1, on_sig);
		(void)write(ready[1], "r", 1);
		for (;;)
			(void)pause();
	}
	if (group)
		(void)setpgid(c, c);
	(void)close(ready[1]);
	if (read(ready[0], &ch, 1) != 1)
		pc_unresolved("a child did not start");
	(void)close(ready[0]);
	return c;
}

/* Collects what arrived on the pipe within ms milliseconds. */
static int got(int fd, long ms)
{
	struct timespec ts = { 0, 10000000 };
	int n = 0, fl;
	long waited;
	char c;

	fl = fcntl(fd, F_GETFL);
	(void)fcntl(fd, F_SETFL, fl | O_NONBLOCK);
	for (waited = 0; waited < ms; waited += 10) {
		while (read(fd, &c, 1) == 1)
			n++;
		(void)nanosleep(&ts, NULL);
	}
	while (read(fd, &c, 1) == 1)
		n++;
	(void)fcntl(fd, F_SETFL, fl);
	return n;
}

static void end(pid_t c)
{
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
}

int main(void)
{
	int p[2];
	pid_t c, gone;

	pc_begin("XSH.kill.002");
	pc_check(pipe(p) == 0, "pipe failed");
	gone = fork();
	pc_check(gone != -1, "fork failed");
	if (gone == 0)
		_exit(0);
	pc_check(waitpid(gone, NULL, 0) == gone, "waitpid failed");
	c = kid(p, 0);
	pc_test();
	{
		int r = kill(c, 0), e = errno;
		int n = got(p[0], 200);
		end(c);
		pc_check(r == 0, "kill(child, 0) returned %d (%s)", r, pc_errname(e));
		pc_check(n == 0, "kill(child, 0) delivered a signal");
	}
	pc_expect_err(kill(gone, 0), ESRCH, "kill(<reaped child>, 0)");
	pc_pass();
}
