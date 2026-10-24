/*
 * kill() with pid > 0 sends the signal to that process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill.001
 * MUTANT    kill.drops
 * VARIANT   kill.late
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
 * Children that report each SIGURG or SIGUSR1 they get as one byte on a
 * pipe. They live until the test's lifeline pipe closes (the test holds
 * its only write end), so none outlives the test, even in another group.
 * In a child of the test (kid_fail_exit set), setup failures exit 98
 * instead of reporting a result.
 */
static int kid_w = -1, life[2] = { -1, -1 }, kid_fail_exit;

static void on_sig(int sig)
{
	char c = sig == SIGURG ? 'u' : '1';
	(void)write(kid_w, &c, 1);
}

static void kid_fail(const char *what)
{
	if (kid_fail_exit)
		_exit(98);
	pc_unresolved("%s", what);
}

/* In a kid: install handlers, say ready, wait for the lifeline to close. */
_Noreturn static void kid_body(int p[2], int ready_w)
{
	struct sigaction sa;
	char c;

	kid_w = p[1];
	(void)close(p[0]);
	(void)close(life[1]);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_sig;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGURG, &sa, NULL) != 0 || sigaction(SIGUSR1, &sa, NULL) != 0)
		_exit(99);
	(void)write(ready_w, "r", 1);
	(void)close(ready_w);
	while (read(life[0], &c, 1) == -1 && errno == EINTR)
		;
	_exit(0);
}

/* group: 0 stays in our group, 1 leads a group of its own, -g joins group g. */
static pid_t kid(int p[2], pid_t group)
{
	int ready[2];
	pid_t c;
	char ch;

	if (life[0] == -1 && pipe(life) != 0)
		kid_fail("pipe failed");
	if (pipe(ready) != 0)
		kid_fail("pipe failed");
	c = fork();
	if (c == -1)
		kid_fail("fork failed");
	if (c == 0) {
		(void)close(ready[0]);
		if (group == 1 && setpgid(0, 0) != 0)
			_exit(99);
		if (group < 0 && setpgid(0, -group) != 0)
			_exit(99);
		kid_body(p, ready[1]);
	}
	if (group == 1)
		(void)setpgid(c, c);
	(void)close(ready[1]);
	if (read(ready[0], &ch, 1) != 1) {
		(void)kill(c, SIGKILL);
		kid_fail("a child did not start");
	}
	(void)close(ready[0]);
	return c;
}

/* Counts bytes on fd, waiting up to 5 s for want of them, then 200 ms more for extras. */
static int got_until(int fd, int want)
{
	struct timespec ts = { 0, 10000000 };
	int n = 0, fl, extra = 0;
	long waited = 0;
	char c;

	fl = fcntl(fd, F_GETFL);
	(void)fcntl(fd, F_SETFL, fl | O_NONBLOCK);
	while (waited < 5000 && extra < 200) {
		while (read(fd, &c, 1) == 1)
			n++;
		if (n >= want)
			extra += 10;
		(void)nanosleep(&ts, NULL);
		waited += 10;
	}
	while (read(fd, &c, 1) == 1)
		n++;
	(void)fcntl(fd, F_SETFL, fl);
	return n;
}

static int got(int fd, long ms)	/* for checks that nothing arrives */
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
	pid_t c;

	pc_begin("XSH.kill.001");
	pc_check(pipe(p) == 0, "pipe failed");
	c = kid(p, 0);
	pc_test();
	{
		int r = kill(c, SIGUSR1);
		int n = got_until(p[0], 1);
		end(c);
		pc_check(r == 0, "kill() returned %d (%s)", r, pc_errname(errno));
		pc_check(n == 1, "the child got %d signals, expected 1", n);
	}
	pc_pass();
}
