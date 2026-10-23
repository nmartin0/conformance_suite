/*
 * psiginfo() writes the same line for the signal in pinfo.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.psiginfo.003
 * MUTANT    psiginfo.info_wrong_signal
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
			pc_check(0, "waitpid failed: %s", pc_errname(errno));
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
	SIGTTOU, SIGUSR1, SIGUSR2, SIGURG, SIGWINCH,
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
	"TTOU", "USR1", "USR2", "URG", "WINCH",
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

/* Runs fn with stderr redirected to a pipe; returns what was written. */
static char out[512];

static const char *capture(void (*fn)(void))
{
	int p[2], saved;
	ssize_t k, n = 0;

	pc_check(pipe(p) == 0, "pipe failed");
	saved = dup(2);
	pc_check(saved != -1, "dup failed");
	(void)fflush(stderr);
	pc_check(dup2(p[1], 2) == 2, "dup2 failed");
	fn();
	(void)fflush(stderr);
	(void)dup2(saved, 2);
	(void)close(saved);
	(void)close(p[1]);
	while (n < (ssize_t)sizeof out - 1 && (k = read(p[0], out + n, sizeof out - 1 - (size_t)n)) > 0)
		n += k;
	(void)close(p[0]);
	out[n] = '\0';
	return out;
}

/* 1 if s is "<prefix><non-empty text without newline>\n". */
static int one_line(const char *s, const char *prefix)
{
	size_t n = strlen(prefix), len = strlen(s);

	if (strncmp(s, prefix, n) != 0 || len < n + 2 || s[len - 1] != '\n')
		return 0;
	return memchr(s + n, '\n', len - n - 1) == NULL;
}

static siginfo_t si;

static void by_info(void)
{
	psiginfo(&si, "pc");
}

static void by_num(void)
{
	psignal(SIGUSR1, "pc");
}

int main(void)
{
	sigset_t s;
	char a[512];
	const char *b;

	pc_begin("XSH.psiginfo.003");
	sigemptyset(&s);
	sigaddset(&s, SIGUSR1);
	pc_check(sigprocmask(SIG_BLOCK, &s, NULL) == 0 && kill(getpid(), SIGUSR1) == 0 &&
	    sigwaitinfo(&s, &si) == SIGUSR1, "could not get a siginfo_t for SIGUSR1");
	pc_test();
	strcpy(a, capture(by_info));
	b = capture(by_num);
	pc_check(one_line(a, "pc: "), "psiginfo() wrote \"%s\"", a);
	pc_check(strcmp(a, b) == 0, "psiginfo() wrote \"%s\" but psignal() \"%s\"", a, b);
	pc_pass();
}
