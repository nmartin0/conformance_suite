/*
 * Timers are not inherited.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.012
 * MUTANT    fork.child_inherits_timers
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


/*
 * Waits for the child; returns its exit status (0 = all its checks held).
 * A child that did not exit normally is UNRESOLVED.
 */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (!WIFEXITED(st))
		pc_unresolved("the child was killed by signal %d", WIFSIGNALED(st) ? WTERMSIG(st) : 0);
	return WEXITSTATUS(st);
}

int main(void)
{
	struct sigevent ev;
	struct itimerspec its;
	sigset_t s;
	timer_t t;
	pid_t r;
	int code;

	pc_begin("XSH.fork.012");
	memset(&ev, 0, sizeof ev);
	ev.sigev_notify = SIGEV_SIGNAL;
	ev.sigev_signo = SIGUSR1;
	pc_check(timer_create(CLOCK_REALTIME, &ev, &t) == 0, "timer_create failed");
	/* SIGUSR1 blocked (inherited): an inherited timer shows as pending. */
	sigemptyset(&s);
	sigaddset(&s, SIGUSR1);
	pc_check(sigprocmask(SIG_BLOCK, &s, NULL) == 0, "sigprocmask failed");
	memset(&its, 0, sizeof its);
	its.it_value.tv_nsec = 300000000;	/* 0.3 s, armed before fork() */
	pc_check(timer_settime(t, 0, &its, NULL) == 0, "timer_settime failed");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		struct timespec ts = { 0, 800000000 };
		sigset_t pend;
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
		if (sigpending(&pend) != 0)
			_exit(9);
		_exit(sigismember(&pend, SIGUSR1) == 1 ? 1 : 0);
	}
	code = reap(r);
	pc_check(code == 0, "a timer the parent armed before fork() signalled the child");
	{
		/* The parent's own timer did fire: the setup worked. */
		sigset_t pend;
		pc_check(sigpending(&pend) == 0, "sigpending failed");
		if (sigismember(&pend, SIGUSR1) != 1)
			pc_unresolved("the parent's timer never fired, so the check proves nothing");
	}
	pc_pass();
}
