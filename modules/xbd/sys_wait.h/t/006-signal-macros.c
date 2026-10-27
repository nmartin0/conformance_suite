/*
 * For children killed by SIGKILL, SIGTERM, SIGHUP, SIGINT, SIGUSR1,
 * SIGUSR2, SIGPIPE, SIGABRT, SIGSEGV, SIGFPE, SIGILL and SIGQUIT with
 * default actions, WIFSIGNALED is true, WTERMSIG gives the signal,
 * WIFEXITED, WIFSTOPPED and (when defined) WIFCONTINUED are false, and for
 * the first seven, whose default action creates no core image, WCOREDUMP
 * is zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_wait.h.006
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t fired;

static void on_alarm(int sig)
{
	(void)sig;
	fired = 1;
}

/* A child that dies of signal sig, with core files switched off. */
static pid_t killed_by(int sig)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		struct rlimit rl;
		sigset_t s;

		rl.rlim_cur = 0;
		rl.rlim_max = 0;
		(void)setrlimit(RLIMIT_CORE, &rl);
		(void)signal(sig, SIG_DFL);
		sigemptyset(&s);
		sigaddset(&s, sig);
		(void)sigprocmask(SIG_UNBLOCK, &s, NULL);
		(void)raise(sig);
		_exit(99);
	}
	return r;
}

static void judge(int sig, const char *name, int core_attempted_ok)
{
	pid_t c = killed_by(sig);
	int st = 0;

	pc_check(waitpid(c, &st, 0) == c, "waitpid for the child killed by %s failed (%s)", name, pc_errname(errno));
	pc_check(WIFSIGNALED(st), "WIFSIGNALED is false for a child killed by %s (status %#x)", name, (unsigned)st);
	pc_check(WTERMSIG(st) == sig, "WTERMSIG is %d for a child killed by %s (%d)", WTERMSIG(st), name, sig);
	pc_check(!WIFEXITED(st) && !WIFSTOPPED(st), "WIFEXITED or WIFSTOPPED is true for a child killed by %s", name);
#ifdef WIFCONTINUED
	pc_check(!WIFCONTINUED(st), "WIFCONTINUED is true for a child killed by %s", name);
#endif
#ifdef WCOREDUMP
	if (!core_attempted_ok)
		pc_check(WCOREDUMP(st) == 0, "WCOREDUMP is true for a child killed by %s, whose default action makes no core image", name);
#else
	(void)core_attempted_ok;
#endif
}

int main(void)
{
	struct sigaction sa;

	pc_begin("XBD.sys_wait.h.006");
#ifndef WCOREDUMP
	pc_unresolved("WCOREDUMP is missing (the header check reports it)");
#endif
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(60);
	pc_test();
	/* Default action: terminate, no core image. */
	judge(SIGKILL, "SIGKILL", 0);
	judge(SIGTERM, "SIGTERM", 0);
	judge(SIGHUP, "SIGHUP", 0);
	judge(SIGINT, "SIGINT", 0);
	judge(SIGUSR1, "SIGUSR1", 0);
	judge(SIGUSR2, "SIGUSR2", 0);
	judge(SIGPIPE, "SIGPIPE", 0);
	/* Default action: abnormal termination; a core image may or may not be made. */
	judge(SIGABRT, "SIGABRT", 1);
	judge(SIGSEGV, "SIGSEGV", 1);
	judge(SIGFPE, "SIGFPE", 1);
	judge(SIGILL, "SIGILL", 1);
	judge(SIGQUIT, "SIGQUIT", 1);
	pc_check(!fired, "the guard alarm fired");
	pc_pass();
}
