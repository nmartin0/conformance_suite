/*
 * exit() status: the full value in SIGCHLD's siginfo_t.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.012
 * MUTANT    exit.status_wrong
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Waits for pid and returns its raw status; anything odd is UNRESOLVED. */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	return st;
}

static pid_t spawn(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	return r;
}

static volatile sig_atomic_t got;
static siginfo_t info;		/* read only after the handler has run */

static void on_chld(int sig, siginfo_t *si, void *uc)
{
	(void)sig;
	(void)uc;
	info = *si;
	got = 1;
}

/* SIGCHLD may come a little after the child can be waited for. */
static void await_chld(void)
{
	struct timespec ts = { 0, 10000000 };
	int i;

	for (i = 0; i < 100 && !got; i++)
		(void)nanosleep(&ts, NULL);
}

int main(void)
{
	struct sigaction sa;
	sigset_t blk, old;
	pid_t c;
	int st;

	pc_begin("XSH.exit.012");
	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = on_chld;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGCHLD, &sa, NULL) == 0, "sigaction failed");
	sigemptyset(&blk);
	sigaddset(&blk, SIGCHLD);
	pc_check(sigprocmask(SIG_BLOCK, &blk, &old) == 0, "sigprocmask failed");
	pc_test();
	c = spawn();
	if (c == 0)
		exit(0x1234);
	{
		siginfo_t si;
		memset(&si, 0, sizeof si);
		if (waitid(P_PID, (id_t)c, &si, WEXITED | WNOWAIT) != 0)
			pc_unresolved("waitid failed: %s", pc_errname(errno));
	}
	pc_check(sigprocmask(SIG_SETMASK, &old, NULL) == 0, "sigprocmask failed");
	await_chld();
	pc_check(got, "no SIGCHLD within 1 s of the child's exit()");
	pc_check(info.si_status == 0x1234, "si_status %#x, expected 0x1234", (unsigned)info.si_status);
	st = reap(c);
	(void)st;
	pc_pass();
}
