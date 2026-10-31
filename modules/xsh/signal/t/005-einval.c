/*
 * signal() fails with SIG_ERR and EINVAL for bad signals and uncatchable ones.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.signal.005
 * MUTANT    signal.no_einval
 * VARIANT   signal.block_all
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

/* A signal number above every valid one: SIGRTMAX is below sysconf(_SC_NSIG) (Issue 8); INT_MAX where that is missing. */
static int big_sig(void)
{
#ifdef _SC_NSIG
	long n = sysconf(_SC_NSIG);

	if (n > 0 && n <= INT_MAX)
		return (int)n;
#endif
	return INT_MAX;
}

static void h(int sig)
{
	(void)sig;
}

int main(void)
{
	struct { int sig; int ign; const char *what; } c[] = {
		{ -1, 0, "signal -1" }, { 0, 0, "a signal number above every valid one" },
		{ SIGKILL, 0, "catching SIGKILL" }, { SIGSTOP, 1, "ignoring SIGSTOP" },
	};
	size_t i;

	pc_begin("XSH.signal.005");
	c[1].sig = big_sig();
	{
		/* The number must not be a signal here; if it is, sysconf() is at fault (XSH.sysconf.015), not signal(). */
		struct sigaction old;

		if (sigaction(c[1].sig, NULL, &old) == 0)
			pc_unresolved("signal %d, from sysconf(_SC_NSIG), is a supported signal here", c[1].sig);
	}
	pc_test();
	for (i = 0; i < sizeof c / sizeof c[0]; i++) {
		void (*r)(int);
		errno = 0;
		r = signal(c[i].sig, c[i].ign ? SIG_IGN : h);
		pc_check(r == SIG_ERR, "%s: signal() did not return SIG_ERR", c[i].what);
		pc_check(errno == EINVAL, "%s: errno is %s, expected EINVAL", c[i].what, pc_errname(errno));
	}
	pc_pass();
}
