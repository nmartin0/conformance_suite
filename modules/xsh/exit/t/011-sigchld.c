/*
 * exit() ends the process as process termination does: SIGCHLD and status.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.011
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

static volatile sig_atomic_t got, code, status, pid;

static void on_chld(int sig, siginfo_t *si, void *uc)
{
	(void)sig;
	(void)uc;
	got = 1;
	code = si->si_code;
	status = si->si_status;
	pid = (sig_atomic_t)si->si_pid;
}

int main(void)
{
	struct sigaction sa;
	sigset_t blk, old;
	pid_t c;
	int st;

	pc_begin("XSH.exit.011");
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
		exit(5);
	/* SIGCHLD stays blocked until the child is a zombie, then is delivered. */
	{
		siginfo_t si;
		memset(&si, 0, sizeof si);
		if (waitid(P_PID, (id_t)c, &si, WEXITED | WNOWAIT) != 0)
			pc_unresolved("waitid failed: %s", pc_errname(errno));
	}
	pc_check(sigprocmask(SIG_SETMASK, &old, NULL) == 0, "sigprocmask failed");
	pc_check(got, "no SIGCHLD after the child called exit()");
	pc_check(code == CLD_EXITED, "si_code %d, expected CLD_EXITED", (int)code);
	pc_check(pid == (sig_atomic_t)c, "si_pid %ld, expected %ld", (long)pid, (long)c);
	pc_check(status == 5, "si_status %d, expected 5", (int)status);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 5, "waitpid status %#x, expected exit 5", st);
	pc_pass();
}
