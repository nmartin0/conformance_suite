/*
 * abort() sends SIGABRT to the calling thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.abort.005
 * MUTANT    abort.to_process
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <wchar.h>

static void catch_sig(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* The status of child c once it ends; FAIL (child killed) after 10 s. */
static int end_of(pid_t c)
{
	int st = 0, waited;
	pid_t r;

	for (waited = 0; waited <= 10000; waited += 10) {
		r = waitpid(c, &st, WNOHANG);
		if (r == c)
			return st;
		if (r == -1 && errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
		sleep_ms(10);
	}
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
	pc_check(0, "the child did not end within 10 s");
	return 0;
}

/* Every signal the header names, and how many. */
static const int table[] = {
	SIGABRT, SIGALRM, SIGBUS, SIGCHLD, SIGCONT, SIGFPE, SIGHUP, SIGILL, SIGINT,
	SIGKILL, SIGPIPE, SIGQUIT, SIGSEGV, SIGSTOP, SIGTERM, SIGTSTP, SIGTTIN,
	SIGTTOU, SIGUSR1, SIGUSR2,
#ifdef SIGURG
	SIGURG,
#endif
#ifdef SIGWINCH
	SIGWINCH,
#endif
#ifdef SIGSYS
	SIGSYS,
#endif
#ifdef SIGTRAP
	SIGTRAP,
#endif
#ifdef SIGVTALRM
	SIGVTALRM,
#endif
#ifdef SIGXCPU
	SIGXCPU,
#endif
#ifdef SIGXFSZ
	SIGXFSZ,
#endif
};
static const char *const table_name[] = {
	"ABRT", "ALRM", "BUS", "CHLD", "CONT", "FPE", "HUP", "ILL", "INT",
	"KILL", "PIPE", "QUIT", "SEGV", "STOP", "TERM", "TSTP", "TTIN",
	"TTOU", "USR1", "USR2",
#ifdef SIGURG
	"URG",
#endif
#ifdef SIGWINCH
	"WINCH",
#endif
#ifdef SIGSYS
	"SYS",
#endif
#ifdef SIGTRAP
	"TRAP",
#endif
#ifdef SIGVTALRM
	"VTALRM",
#endif
#ifdef SIGXCPU
	"XCPU",
#endif
#ifdef SIGXFSZ
	"XFSZ",
#endif
};
#define NTABLE ((int)(sizeof table / sizeof table[0]))

/* Forks a child that runs fn (which should end in abort()). */
static pid_t run(void (*fn)(void))
{
	pid_t c = fork();

	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		struct rlimit rl = { 0, 0 };
		(void)setrlimit(RLIMIT_CORE, &rl);	/* no core files */
		fn();
		_exit(10);	/* abort() returned */
	}
	return c;
}

static void expect_sigabrt(int st, const char *what)
{
	pc_check(!(WIFEXITED(st) && WEXITSTATUS(st) == 10), "%s: abort() returned", what);
	pc_check(WIFSIGNALED(st) && WTERMSIG(st) == SIGABRT, "%s: status %#x, expected termination by SIGABRT", what, st);
}

#define BLOCK_IN_B 0

static _Thread_local int is_b;

static void h(int sig)
{
	(void)sig;
	_exit(is_b ? 7 : 8);
}

static void *caller(void *arg)
{
	sigset_t s;

	(void)arg;
	is_b = 1;
	if (BLOCK_IN_B) {
		/* abort() must override this block */
		sigemptyset(&s);
		sigaddset(&s, SIGABRT);
		(void)pthread_sigmask(SIG_BLOCK, &s, NULL);
	}
	abort();
	return NULL;
}

static void threaded(void)
{
	struct sigaction sa;
	pthread_t b;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = h;
	sigemptyset(&sa.sa_mask);
	(void)sigaction(SIGABRT, &sa, NULL);
	if (pthread_create(&b, NULL, caller, NULL) != 0)
		_exit(99);
	for (;;)
		sleep_ms(1000);
}

int main(void)
{
	int st;

	pc_begin("XSH.abort.005");
	pc_test();
	st = end_of(run(threaded));
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("pthread_create failed in the child");
	pc_check(!(WIFEXITED(st) && WEXITSTATUS(st) == 8), "the SIGABRT handler ran in the main thread");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 7, "the child ended with status %#x", st);
	pc_pass();
}
