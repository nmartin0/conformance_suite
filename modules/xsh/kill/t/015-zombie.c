/*
 * kill() to a zombie succeeds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill.015
 * MUTANT    kill.zombie_esrch
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Installs fn for sig (FAIL or UNRESOLVED by phase). */
static void catch_sig(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/* A set holding the listed signals (end the list with 0). */
static sigset_t set_of(int first, ...)
{
	sigset_t s;
	va_list ap;
	int sig;

	sigemptyset(&s);
	va_start(ap, first);
	for (sig = first; sig != 0; sig = va_arg(ap, int))
		sigaddset(&s, sig);
	va_end(ap);
	return s;
}

int main(void)
{
	siginfo_t si;
	pid_t c;

	pc_begin("XSH.kill.015");
	c = fork();
	pc_check(c != -1, "fork failed");
	if (c == 0)
		_exit(0);
	memset(&si, 0, sizeof si);
	pc_check(waitid(P_PID, (id_t)c, &si, WEXITED | WNOWAIT) == 0, "waitid(WNOWAIT) failed");
	pc_test();
	pc_check(kill(c, 0) == 0, "kill(<zombie>, 0) failed with %s", pc_errname(errno));
	(void)waitpid(c, NULL, 0);
	pc_pass();
}
