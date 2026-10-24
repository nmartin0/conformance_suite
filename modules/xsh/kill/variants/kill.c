/*
 * kill.c - conforming variants of kill(), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "late": a signal for another process, or for a process group the sender
 * is not in, reaches its target about a second after kill() returns.
 * XSH 2.4.1 puts no bound on the time a signal spends pending between
 * generation and delivery; only a signal for the calling process itself
 * has to be delivered before kill() returns (XSH kill() DESCRIPTION),
 * and those are sent at once here. Errors are still decided at the call:
 * the null signal checks that the target exists and may be signalled
 * (SIGCONT within the session needs no permission, as kill() says), and
 * only then is the send handed to a helper process with the sender's
 * credentials and all signals blocked, which sends it after the delay.
 *
 * The helper generates the signal late rather than holding it pending, so
 * a target that blocks the signal and looks at its pending set (with
 * sigpending()) before the delay ends could tell the two apart. Tests
 * declare this variant only where the target does not inspect its pending
 * set in that window (it catches the signal, or waits for it with
 * sigsuspend() or sigwait()), so they cannot. SIGKILL and SIGSTOP are
 * sent at once (tests reap with them right away).
 */
#define LATE_MS 1000

static int valid_sig(int sig)
{
	sigset_t s;

	sigemptyset(&s);
	return sig > 0 && sigaddset(&s, sig) == 0;
}

static void send_late(int (*real)(pid_t, int), pid_t pid, int sig)
{
	pid_t c = fork(), g;
	sigset_t all;

	if (c == -1) {
		(void)real(pid, sig);	/* no helper: send it now */
		return;
	}
	if (c == 0) {
		sigfillset(&all);
		(void)sigprocmask(SIG_BLOCK, &all, NULL);
		g = fork();
		if (g == -1)
			(void)real(pid, sig);
		if (g == 0) {
			struct timespec ts = { LATE_MS / 1000, (LATE_MS % 1000) * 1000000L };
			while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
				;
			(void)real(pid, sig);
		}
		_exit(0);
	}
	while (waitpid(c, NULL, 0) == -1 && errno == EINTR)
		;
}

int kill(pid_t pid, int sig)
{
	const char *v = variant_of("kill.");
	int e = errno, r;
	REAL(kill);

	if (!IS("late") || !valid_sig(sig) || sig == SIGKILL || sig == SIGSTOP)
		return real_kill(pid, sig);
	if (!(pid > 0 && pid != getpid()) && !(pid < -1 && -pid != getpgrp()))
		return real_kill(pid, sig);
	r = real_kill(pid, 0);
	if (r != 0 && !(errno == EPERM && sig == SIGCONT && pid > 0 && getsid(pid) == getsid(0)))
		return r;
	send_late(real_kill, pid, sig);
	errno = e;
	return 0;
}
