/*
 * The realtime signals' default action terminates the process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.4.3-signal-actions.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void catch_sig(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

static void catch_info(int sig, void (*fn)(int, siginfo_t *, void *))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = fn;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/* Sets SIG_DFL or SIG_IGN (or a plain handler) with sigaction(). */
static void set_action(int sig, void (*fn)(int))
{
	catch_sig(sig, fn);
}

static void mask(int how, int sig)
{
	sigset_t s;

	sigemptyset(&s);
	sigaddset(&s, sig);
	pc_check(pthread_sigmask(how, &s, NULL) == 0, "pthread_sigmask failed");
}

static int is_pending(int sig)
{
	sigset_t p;

	pc_check(sigpending(&p) == 0, "sigpending failed");
	return sigismember(&p, sig) == 1;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Children made here are killed when the test exits, however it ends. */
static pid_t kids[16];
static int nkids;

static void kill_kids(void)
{
	int i;

	for (i = 0; i < nkids; i++)
		if (kids[i] > 0) {
			(void)kill(kids[i], SIGKILL);
			(void)kill(kids[i], SIGCONT);
		}
}

static void track(pid_t c)
{
	if (nkids == 0)
		(void)atexit(kill_kids);
	if (nkids < 16)
		kids[nkids++] = c;
}

static void kill_child(pid_t c)
{
	(void)kill(c, SIGKILL);
	(void)kill(c, SIGCONT);
	while (waitpid(c, NULL, 0) == -1 && errno == EINTR)
		;
}

/*
 * Waits up to ms for a status change of c that opts select (WUNTRACED
 * adds stops); 1 with *st set, or 0 if none came in time.
 */
static int wait_by(pid_t c, int opts, long ms, int *st)
{
	long waited;
	pid_t r;

	for (waited = 0; waited <= ms; waited += 10) {
		r = waitpid(c, st, opts | WNOHANG);
		if (r == c)
			return 1;
		if (r == -1 && errno != EINTR)
			pc_check(0, "waitpid failed: %s", pc_errname(errno));
		sleep_ms(10);
	}
	return 0;
}

/* The status of c once it ends (or stops); FAIL if neither within 10 s. */
static int end_of(pid_t c, const char *what)
{
	int st = 0;

	if (!wait_by(c, WUNTRACED, 10000, &st)) {
		kill_child(c);
		pc_check(0, "%s: the child did not end within 10 s", what);
	}
	if (WIFSTOPPED(st))
		kill_child(c);
	return st;
}

/* Waits until c is stopped by sig; FAIL (child killed) otherwise. */
static void await_stop(pid_t c, int sig)
{
	int st = 0;

	if (!wait_by(c, WUNTRACED, 10000, &st)) {
		kill_child(c);
		pc_check(0, "the child was not stopped within 10 s");
	}
	if (!WIFSTOPPED(st)) {
		pc_check(0, "the child ended (status %#x) instead of stopping", st);
	}
	if (WSTOPSIG(st) != sig) {
		kill_child(c);
		pc_check(0, "the child was stopped by %d, expected %d", WSTOPSIG(st), sig);
	}
}

/* Reads one byte from fd within ms milliseconds: the byte, or -1. */
static int read_within(int fd, long ms)
{
	long waited;
	char ch;
	int fl = fcntl(fd, F_GETFL);

	(void)fcntl(fd, F_SETFL, fl | O_NONBLOCK);
	for (waited = 0; waited <= ms; waited += 10) {
		ssize_t k = read(fd, &ch, 1);
		if (k == 1) {
			(void)fcntl(fd, F_SETFL, fl);
			return (unsigned char)ch;
		}
		if (k == 0)
			break;
		sleep_ms(10);
	}
	(void)fcntl(fd, F_SETFL, fl);
	return -1;
}

/* Waits (no deadline: the driver's limit covers a hang) for c's 'a'. */
static void started(int fd, pid_t c)
{
	char ch = 0;
	ssize_t k;

	while ((k = read(fd, &ch, 1)) == -1 && errno == EINTR)
		;
	if (k != 1 || ch != 'a') {
		kill_child(c);
		pc_unresolved("the child did not start (%c)", k == 1 ? ch : '-');
	}
}

/*
 * Runs fn in a child (in the test's process group, which is not orphaned)
 * and returns its pid; report[0] is the read end of a pipe whose write end
 * the child has as fd report[1].
 */
static pid_t child(void (*fn)(int), int report[2])
{
	pid_t c;

	pc_check(pipe(report) == 0, "pipe failed");
	c = fork();
	pc_check(c != -1, "fork failed");
	if (c == 0) {
		(void)close(report[0]);
		fn(report[1]);
		_exit(0);
	}
	track(c);
	(void)close(report[1]);
	return c;
}

static void say(int fd, char ch)
{
	(void)write(fd, &ch, 1);
}

int main(void)
{
	const int sigs[] = { SIGRTMIN, SIGRTMAX };
	static const char *const name[] = { "SIGRTMIN", "SIGRTMAX" };
	size_t i;

	pc_begin("XSH.2.4.3-signal-actions.003");
	pc_test();
	for (i = 0; i < sizeof sigs / sizeof sigs[0]; i++) {
		pid_t c = fork();
		int st;
		pc_check(c != -1, "fork failed");
		if (c == 0) {
			struct rlimit rl = { 0, 0 };
			struct sigaction sa;
			(void)setrlimit(RLIMIT_CORE, &rl);	/* no core files */
			memset(&sa, 0, sizeof sa);
			sa.sa_handler = SIG_DFL;
			(void)sigaction(sigs[i], &sa, NULL);
			(void)kill(getpid(), sigs[i]);
			_exit(10);
		}
		track(c);
		st = end_of(c, name[i]);
		pc_check(!(WIFEXITED(st) && WEXITSTATUS(st) == 10), "%s at SIG_DFL did not terminate the process", name[i]);
		pc_check(WIFSIGNALED(st) && WTERMSIG(st) == sigs[i], "%s: status %#x, expected termination by that signal", name[i], st);
	}
	pc_pass();
}
