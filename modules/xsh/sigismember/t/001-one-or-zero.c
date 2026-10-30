/*
 * sigismember() returns exactly 1 for a member and 0 otherwise.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigismember.001
 * MUTANT    sigismember.returns_nonzero
 * MUTANT    sigismember.inverted
 * VARIANT   sigfillset.omits_extra
 * VARIANT   sigemptyset.keeps_extra
 * VARIANT   sigaddset.rt_minimum
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

int main(void)
{
	pc_begin("XSH.sigismember.001");
	{
		sigset_t s, f;
		int a, b, c, d;

		pc_check(sigemptyset(&s) == 0 && sigaddset(&s, SIGUSR2) == 0 && sigfillset(&f) == 0, "set up failed");
		pc_test();
		a = sigismember(&s, SIGUSR2);
		b = sigismember(&s, SIGUSR1);
#ifdef SIGRTMIN
		c = sigismember(&s, SIGRTMIN);
#else
		c = 0;
		pc_note("<signal.h> does not define SIGRTMIN; the real-time non-member is not checked here");
#endif
		d = sigismember(&f, SIGUSR1);
		(void)c;
		pc_check(a == 1, "sigismember() of a member gave %d, expected 1", a);
		pc_check(b == 0 && c == 0, "sigismember() of non-members gave %d and %d, expected 0", b, c);
		pc_check(d == 1, "sigismember() on a full set gave %d, expected 1", d);
	}
	pc_pass();
}
