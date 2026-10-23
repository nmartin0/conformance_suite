/*
 * SIGCONT may be sent to any process in the sender's session.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill.011
 * MUTANT    kill.sigcont_checked
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

/*
 * In a child of a privileged test: become user and group u, and prove
 * that no privilege is left before signalling anything (both IDs are u,
 * and the privileged parent cannot be signalled). Exits 99 otherwise.
 */
static void become(uid_t u)
{
	if (setgid((gid_t)u) != 0 || setuid(u) != 0)
		_exit(99);
	if (getuid() != u || geteuid() != u || getgid() != (gid_t)u || getegid() != (gid_t)u)
		_exit(99);
	/* The proof is setup: planted faults (CI) stay out of it. */
	(void)setenv("PC_PHASE", "setup", 1);
	if (getppid() != 1 && (kill(getppid(), 0) != -1 || errno != EPERM))
		_exit(99);
	(void)setenv("PC_PHASE", "test", 1);
}

static void become_nobody(void)
{
	become(65534);
}

/* The otherwise unused user ID from the profile (have kill_uid U). */
static uid_t kill_uid(void)
{
	const char *v = getenv("PC_HAVE_KILL_UID");
	char *end;
	unsigned long u;

	if (v == NULL || *v == '\0')
		pc_unresolved("the profile names no unused user ID (have kill_uid U)");
	u = strtoul(v, &end, 10);
	if (*end != '\0' || u == 0)
		pc_unresolved("bad kill_uid '%s' in the profile", v);
	return (uid_t)u;
}

static volatile sig_atomic_t cont;

static void on_cont(int sig)
{
	(void)sig;
	cont++;
}

int main(void)
{
	pid_t s;
	int st;

	pc_begin("XSH.kill.011");
	catch_sig(SIGCONT, on_cont);
	pc_test();
	s = fork();
	if (s == -1)
		pc_unresolved("fork failed");
	if (s == 0) {
		become_nobody();
		_exit(kill(getppid(), SIGCONT) == 0 ? 0 : errno == EPERM ? 10 : 11);
	}
	{
		pid_t w;
		while ((w = waitpid(s, &st, 0)) == -1 && errno == EINTR)
			;	/* the SIGCONT handler may interrupt the wait */
		if (w != s)
			pc_unresolved("waitpid failed");
	}
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("cannot run as user 65534");
	{
		struct timespec ts = { 0, 100000000 };
		int i;
		for (i = 0; i < 20 && cont == 0; i++)
			(void)nanosleep(&ts, NULL);
	}
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) != 10, "kill(SIGCONT) within the session failed with EPERM");
	pc_check(WEXITSTATUS(st) == 0, "kill(SIGCONT) within the session failed");
	pc_check(cont == 1, "the SIGCONT handler ran %d times, expected once", (int)cont);
	pc_pass();
}
