/*
 * Timers are not inherited.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.012
 * MUTANT    fork.child_gets_timer_signal
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

static volatile sig_atomic_t got;

static void on_usr1(int sig)
{
	(void)sig;
	got = 1;
}

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
	timer_t t;
	pid_t r;
	int code;

	pc_begin("XSH.fork.012");
	memset(&ev, 0, sizeof ev);
	ev.sigev_notify = SIGEV_SIGNAL;
	ev.sigev_signo = SIGUSR1;
	pc_check(timer_create(CLOCK_REALTIME, &ev, &t) == 0, "timer_create failed");
	signal(SIGUSR1, SIG_IGN);	/* the parent ignores its own timer */
	memset(&its, 0, sizeof its);
	its.it_value.tv_nsec = 200000000;	/* 0.2 s */
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		struct sigaction sa;
		struct timespec ts = { 0, 600000000 };
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = on_usr1;
		sigemptyset(&sa.sa_mask);
		(void)sigaction(SIGUSR1, &sa, NULL);
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
		_exit(got ? 1 : 0);
	}
	pc_check(timer_settime(t, 0, &its, NULL) == 0, "timer_settime failed");
	code = reap(r);
	pc_check(code == 0, "the parent's timer signalled the child");
	pc_pass();
}
