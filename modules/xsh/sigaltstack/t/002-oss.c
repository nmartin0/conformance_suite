/*
 * oss reports the alternate stack in effect before the call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaltstack.002
 * MUTANT    sigaltstack.oss_zeroed
 * MUTANT    sigaltstack.oss_flags_lost
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

static char *alt;
static size_t altsz;

static void make_alt(void)
{
	altsz = (size_t)SIGSTKSZ * 4;
	alt = malloc(altsz);
	pc_check(alt != NULL, "malloc failed");
}

static int on_alt(const void *p)
{
	const char *c = p;

	return c >= alt && c < alt + altsz;
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

int main(void)
{
	stack_t a, b, o1, o2;
	char *other;

	pc_begin("XSH.sigaltstack.002");
	make_alt();
	other = malloc(altsz);
	pc_check(other != NULL, "malloc failed");
	a.ss_sp = alt;
	a.ss_size = altsz;
	a.ss_flags = 0;
	b.ss_sp = other;
	b.ss_size = altsz;
	b.ss_flags = 0;
	pc_test();
	pc_check(sigaltstack(&a, &o1) == 0, "the first sigaltstack() failed");
	pc_check(sigaltstack(&b, &o2) == 0, "the second sigaltstack() failed");
	pc_check(o1.ss_flags & SS_DISABLE, "before any alternate stack, ss_flags is %#x, not SS_DISABLE", o1.ss_flags);
	pc_check(o2.ss_sp == alt && o2.ss_size == altsz, "oss gave %p/%zu, expected the first stack %p/%zu", o2.ss_sp, o2.ss_size, (void *)alt, altsz);
	pc_check(!(o2.ss_flags & SS_DISABLE), "oss for an enabled stack says SS_DISABLE");
	pc_pass();
}
