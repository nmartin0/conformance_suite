/*
 * While a signal() handler runs, its signal is reset to SIG_DFL or blocked.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.signal.004
 * MUTANT    signal.neither_reset_nor_block
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

static volatile sig_atomic_t depth, calls, nested;

/* Raises SIGUSR1 again from inside its own handler, once: a reset action ends the process; a signal that is not
   held off re-enters the handler at once. */
static void h(int sig)
{
	depth++;
	calls++;
	if (depth > 1)
		nested = 1;
	else if (calls == 1)
		(void)raise(sig);
	depth--;
}

static int child(void)
{
	if (signal(SIGUSR1, h) == SIG_ERR)
		return 9;
	(void)raise(SIGUSR1);
	/* still here: the action was not reset */
	if (nested)
		return 3;	/* neither reset nor held off */
	return calls == 2 ? 0 : 4;	/* 0: held off, then delivered; 4: held off and lost (allowed) */
}

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH.signal.004");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0)
		_exit(child());
	st = end_of(c);
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGUSR1)
		pc_pass();	/* the action was reset to SIG_DFL */
	pc_check(WIFEXITED(st), "the child ended with status %#x", (unsigned)st);
	pc_check(WEXITSTATUS(st) != 9, "signal() failed");
	pc_check(WEXITSTATUS(st) != 3, "inside the handler SIGUSR1 was neither reset to SIG_DFL nor held off");
	if (WEXITSTATUS(st) == 4)
		pc_note("SIGUSR1 raised inside its handler was not delivered later");
	pc_check(WEXITSTATUS(st) == 0 || WEXITSTATUS(st) == 4, "child status %d", WEXITSTATUS(st));
	pc_pass();
}
