/*
 * Permission compares with the receiver's real and saved user IDs, not its effective one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill.014
 * MUTANT    kill.eperm_hidden
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

int main(void)
{
	int ready[2], res[2], st;
	pid_t target, s;
	char c;

	pc_begin("XSH.kill.014");
	pc_check(pipe(ready) == 0 && pipe(res) == 0, "pipe failed");
	target = fork();
	pc_check(target != -1, "fork failed");
	if (target == 0) {
		/* real 0, effective 65534, saved 0; waits for the end of the test */
		(void)close(ready[0]);
		(void)close(res[1]);
		if (seteuid(65534) != 0)
			_exit(99);
		(void)write(ready[1], "r", 1);
		while (read(res[0], &c, 1) == -1 && errno == EINTR)
			;
		_exit(0);
	}
	(void)close(ready[1]);
	(void)close(res[0]);
	if (read(ready[0], &c, 1) != 1) {
		(void)waitpid(target, NULL, 0);
		pc_unresolved("the target could not seteuid(65534)");
	}
	pc_test();
	s = fork();
	if (s == -1)
		pc_unresolved("fork failed");
	if (s == 0) {
		become_nobody();
		_exit(kill(target, SIGURG) == 0 ? 10 : errno == EPERM ? 0 : 11);
	}
	(void)waitpid(s, &st, 0);
	(void)close(res[1]);	/* lets the target go */
	(void)waitpid(target, NULL, 0);
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("cannot run as user 65534");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) != 10,
	    "a user-65534 process could signal one whose real and saved user IDs are 0");
	pc_check(WEXITSTATUS(st) == 0, "kill() failed with an error other than EPERM");
	pc_pass();
}
