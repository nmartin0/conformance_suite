/*
 * A handler can run on an alternate stack of exactly MINSIGSTKSZ bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaltstack.010
 * VARIANT   sigaltstack.bsd_flags
 * VARIANT   sigaltstack.minsigstksz_32k
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

static char *alt;
static size_t altsz;

static void make_alt(void)
{
	altsz = (size_t)SIGSTKSZ * 4;
	alt = malloc(altsz);
	pc_check(alt != NULL, "malloc failed");
}

/* Address arithmetic as integers: comparing unrelated pointers is undefined. */
static int on_alt(const void *p)
{
	uintptr_t c = (uintptr_t)p, a = (uintptr_t)alt;

	return c >= a && c < a + altsz;
}

static void catch_onstack(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sa.sa_flags = SA_ONSTACK;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

static volatile sig_atomic_t ran;

static void h(int sig)
{
	(void)sig;
	ran = 1;
}

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH.sigaltstack.010");
	make_alt();
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		stack_t ss;
		struct sigaction sa;
		ss.ss_sp = alt + altsz - (size_t)MINSIGSTKSZ;
		ss.ss_size = (size_t)MINSIGSTKSZ;
		ss.ss_flags = 0;
		if (sigaltstack(&ss, NULL) != 0)
			_exit(98);
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = h;
		sa.sa_flags = SA_ONSTACK;
		sigemptyset(&sa.sa_mask);
		(void)sigaction(SIGUSR1, &sa, NULL);
		(void)raise(SIGUSR1);
		_exit(ran ? 0 : 97);
	}
	st = end_of(c);
	if (WIFEXITED(st) && WEXITSTATUS(st) == 98)
		pc_unresolved("sigaltstack() refused a MINSIGSTKSZ stack");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child with a MINSIGSTKSZ alternate stack ended with status %#x%s", st,
	    WIFSIGNALED(st) && WTERMSIG(st) == SIGSEGV ? " (SIGSEGV: the signal frame did not fit)" : "");
	pc_pass();
}
