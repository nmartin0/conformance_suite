/*
 * sigwait() takes a pending signal from the set and clears it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigwait.001
 * MUTANT    sigwait.wrong_sig
 * MUTANT    sigwait.not_cleared
 * MUTANT    sigwait.returns_minus1
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
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

static void block(int sig)
{
	sigset_t s = set_of(sig, 0);

	pc_check(pthread_sigmask(SIG_BLOCK, &s, NULL) == 0, "pthread_sigmask failed");
}

static long now_ms(void)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long)t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}

/*
 * A child that sends sig to this process after ms milliseconds, then
 * exits. The caller reaps it with reap_sender().
 */
static pid_t send_later(int sig, long ms)
{
	pid_t parent = getpid(), c = fork();

	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		struct timespec ts;
		ts.tv_sec = ms / 1000;
		ts.tv_nsec = (ms % 1000) * 1000000L;
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
		(void)kill(parent, sig);
		_exit(0);
	}
	return c;
}

static void reap_sender(pid_t c)
{
	while (waitpid(c, NULL, 0) == -1 && errno == EINTR)
		;
}

static volatile sig_atomic_t ran;

static void h(int sig)
{
	(void)sig;
	ran++;
}

int main(void)
{
	sigset_t s = set_of(SIGUSR1, 0), p;
	int sig = 0, r;

	pc_begin("XSH.sigwait.001");
	catch_sig(SIGUSR1, h);
	block(SIGUSR1);
	pc_check(kill(getpid(), SIGUSR1) == 0, "kill failed");
	pc_test();
	r = sigwait(&s, &sig);
	pc_check(r == 0, "sigwait() returned %d, expected 0", r);
	pc_check(sig == SIGUSR1, "sigwait() stored %d, expected SIGUSR1", sig);
	pc_check(sigpending(&p) == 0 && sigismember(&p, SIGUSR1) != 1, "SIGUSR1 is still pending after sigwait()");
	pc_check(ran == 0, "the handler ran for a signal accepted by sigwait()");
	pc_pass();
}
