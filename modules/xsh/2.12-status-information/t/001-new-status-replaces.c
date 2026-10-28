/*
 * An unreported stop and continuation are replaced by the exit that follows them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.12-status-information.001
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#ifdef WCONTINUED
#define OPTS (WUNTRACED | WCONTINUED)
#else
#define OPTS WUNTRACED
#endif

static pid_t child;

/* On any early end the child (possibly stopped) must not be left behind. */
static void reap(void)
{
	if (child > 0) {
		(void)kill(child, SIGKILL);
		(void)waitpid(child, NULL, 0);
	}
}

static void on_alarm(int s)
{
	(void)s;
	if (child > 0) {
		(void)kill(child, SIGKILL);
		(void)waitpid(child, NULL, 0);
	}
	pc_unresolved("the test timed out");
}

int main(void)
{
	siginfo_t si;
	int st, i, stopped = 0;
	pid_t r;

	pc_begin("XSH.2.12-status-information.001");
	(void)signal(SIGALRM, on_alarm);
	(void)alarm(25);
	child = fork();
	NEED(child != -1, "fork failed");
	if (child == 0) {
		(void)raise(SIGSTOP);
		_exit(5);
	}
	(void)atexit(reap);
	/* Look at the stop without consuming it. */
	for (i = 0; i < 1000 && !stopped; i++) {
		struct timespec ts = { 0, 10000000 };

		si.si_pid = 0;
		NEED(waitid(P_PID, (id_t)child, &si, WSTOPPED | WNOHANG | WNOWAIT) == 0, "waitid failed: %s", pc_errname(errno));
		if (si.si_pid == child)
			stopped = 1;
		else
			(void)nanosleep(&ts, NULL);
	}
	NEED(stopped, "the child did not stop");
	pc_test();
	NEED(kill(child, SIGCONT) == 0, "kill failed");
	si.si_pid = 0;
	pc_check(waitid(P_PID, (id_t)child, &si, WEXITED | WNOWAIT) == 0 && si.si_pid == child, "waitid(WEXITED|WNOWAIT) did not report the exit");
	r = waitpid(child, &st, OPTS);
	pc_check(r == child, "waitpid returned %ld, not the child", (long)r);
	pc_check(WIFEXITED(st), "the status is not an exit (status %d): the stop or continuation was reported instead", st);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 5, "the exit status is %d, not 5", WEXITSTATUS(st));
	errno = 0;
	r = waitpid(child, &st, OPTS);
	pc_check(r == -1 && errno == ECHILD, "a second waitpid returned %ld (%s), not -1 with ECHILD", (long)r, pc_errname(errno));
	child = 0;
	pc_pass();
}
