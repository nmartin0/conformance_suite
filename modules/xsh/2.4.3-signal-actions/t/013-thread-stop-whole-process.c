/*
 * A stop signal delivered to one thread stops the whole process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.4.3-signal-actions.013
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
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

static void set_action(int sig, void (*fn)(int))
{
	pc_check(signal(sig, fn) != SIG_ERR, "signal(%d) failed", sig);
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

/* Reaps c, retrying on EINTR; returns the status. */
static int reap(pid_t c, int opts)
{
	int st = 0;
	pid_t r;

	while ((r = waitpid(c, &st, opts)) == -1 && errno == EINTR)
		;
	pc_check(r == c, "waitpid failed: %s", pc_errname(errno));
	return st;
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

/*
 * Runs fn in a child (its own process group) and returns the child's
 * pid; report[0] is the read end of a pipe whose write end the child has
 * as fd report[1].
 */
static pid_t child(void (*fn)(int), int report[2])
{
	pid_t c;

	pc_check(pipe(report) == 0, "pipe failed");
	c = fork();
	pc_check(c != -1, "fork failed");
	if (c == 0) {
		(void)close(report[0]);
		(void)setpgid(0, 0);
		fn(report[1]);
		_exit(0);
	}
	(void)close(report[1]);
	return c;
}

static void say(int fd, char ch)
{
	(void)write(fd, &ch, 1);
}

/* Waits until c is stopped (WUNTRACED); FAIL if it ends instead. */
static void await_stop(pid_t c, int sig)
{
	int st = reap(c, WUNTRACED);

	pc_check(WIFSTOPPED(st), "the child was not stopped (status %#x)", st);
	pc_check(WSTOPSIG(st) == sig, "the child was stopped by %d, expected %d", WSTOPSIG(st), sig);
}

static void kill_child(pid_t c)
{
	(void)kill(c, SIGKILL);
	(void)kill(c, SIGCONT);
	(void)waitpid(c, NULL, 0);
}

static void *idle(void *arg)
{
	(void)arg;
	for (;;)
		sleep_ms(1000);
	return NULL;
}

static void kid(int fd)
{
	pthread_t b;

	if (pthread_create(&b, NULL, idle, NULL) != 0)
		_exit(99);
	say(fd, 'a');
	sleep_ms(100);
	(void)pthread_kill(b, SIGSTOP);
	for (;;) {
		say(fd, 't');	/* ticks while running */
		sleep_ms(20);
	}
}

int main(void)
{
	int rep[2], during, after;
	pid_t c;

	pc_begin("XSH.2.4.3-signal-actions.013");
	pc_test();
	c = child(kid, rep);
	pc_check(read_within(rep[0], 5000) == 'a', "the child did not start");
	await_stop(c, SIGSTOP);
	{
		/* empty the pipe of ticks made before the stop took effect */
		char buf[256];
		int fl = fcntl(rep[0], F_GETFL);
		(void)fcntl(rep[0], F_SETFL, fl | O_NONBLOCK);
		while (read(rep[0], buf, sizeof buf) > 0)
			;
		(void)fcntl(rep[0], F_SETFL, fl);
	}
	during = read_within(rep[0], 300);
	pc_check(kill(c, SIGCONT) == 0, "kill failed");
	after = read_within(rep[0], 3000);
	kill_child(c);
	pc_check(during == -1, "the main thread kept running after SIGSTOP went to the other thread");
	pc_check(after == 't', "the child did not go on after SIGCONT");
	pc_pass();
}
