/*
 * str2sig() takes decimal numbers and everything sig2str() writes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sig2str.005
 * MUTANT    sig2str.str2sig_no_numbers
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

/*
 * sig2str() into a buffer with a guard area after SIG2STR_MAX bytes, so a
 * name longer than SIG2STR_MAX is a FAIL, not an overflow of the test's
 * own buffer.
 */
#define GUARD 64
static char buf[SIG2STR_MAX + GUARD];

static int s2s(int sig)
{
	int r, i;

	memset(buf, 0x5a, sizeof buf);
	r = sig2str(sig, buf);
	if (r == 0) {
		pc_check(memchr(buf, '\0', SIG2STR_MAX) != NULL, "sig2str(%d) wrote a name of SIG2STR_MAX bytes or more", sig);
		for (i = SIG2STR_MAX; i < SIG2STR_MAX + GUARD; i++)
			pc_check(buf[i] == 0x5a, "sig2str(%d) wrote past SIG2STR_MAX bytes", sig);
	} else
		buf[0] = '\0';
	return r;
}

/* str2sig() of str: the return value, with *n set (or left at -7). */
static int s2n(const char *str, int *n)
{
	*n = -7;
	return str2sig(str, n);
}

int main(void)
{
	pc_begin("XSH.sig2str.005");
	{
		char num[32];
		int s, n, r;

		pc_test();
		(void)snprintf(num, sizeof num, "%d", SIGUSR1);
		r = s2n(num, &n);
		pc_check(r == 0 && n == SIGUSR1, "str2sig(\"%s\") returned %d and stored %d", num, r, n);
		(void)snprintf(num, sizeof num, "%d", SIGRTMIN);
		r = s2n(num, &n);
		pc_check(r == 0 && n == SIGRTMIN, "str2sig(\"%s\") returned %d and stored %d", num, r, n);
		for (s = 1; s <= SIGRTMAX; s++) {
			if (s2s(s) != 0)
				continue;
			r = s2n(buf, &n);
			pc_check(r == 0 && n == s, "str2sig(\"%s\") returned %d and stored %d, expected %d (from sig2str)", buf, r, n, s);
		}
	}
	pc_pass();
}
