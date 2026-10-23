/*
 * An orphaned group with a stopped member gets SIGHUP then SIGCONT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.010
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

static int report_fd = -1;

static void on_hup(int sig)
{
	(void)sig;
	(void)write(report_fd, "H", 1);
}

static void on_cont(int sig)
{
	(void)sig;
	(void)write(report_fd, "C", 1);
}

/* Waits for pid and returns its raw status; anything odd is UNRESOLVED. */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	return st;
}

static pid_t spawn(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	return r;
}

int main(void)
{
	int p[2], st;
	pid_t mid;
	char got[2] = { 0, 0 };

	pc_begin("XSH._Exit.010");
	pc_check(pipe(p) == 0, "pipe failed");
	alarm(20);
	pc_test();
	mid = spawn();
	if (mid == 0) {
		pid_t g;
		if (setpgid(0, 0) != 0)
			_exit(98);	/* a group of its own, in our session */
		g = fork();
		if (g == -1)
			_exit(97);
		if (g == 0) {
			struct sigaction sa;
			memset(&sa, 0, sizeof sa);
			sigemptyset(&sa.sa_mask);
			sa.sa_handler = on_hup;
			(void)sigaction(SIGHUP, &sa, NULL);
			sa.sa_handler = on_cont;
			(void)sigaction(SIGCONT, &sa, NULL);
			report_fd = p[1];
			(void)raise(SIGSTOP);
			{
				struct timespec ts = { 0, 200000000 };
				(void)nanosleep(&ts, NULL);
			}
			_exit(0);
		}
		{
			int s;
			(void)waitpid(g, &s, WUNTRACED);	/* g has stopped */
		}
		_exit(0);	/* orphans g's group */
	}
	(void)close(p[1]);
	st = reap(mid);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the middle process failed (%#x)", st);
	/*
	 * Both must arrive. They are sent SIGHUP first, but the order in
	 * which pending signals reach their handlers is unspecified (XSH 2.4).
	 */
	pc_check(read(p[0], &got[0], 1) == 1, "the stopped orphan got neither SIGHUP nor SIGCONT");
	pc_check(read(p[0], &got[1], 1) == 1, "the stopped orphan got only one of SIGHUP and SIGCONT");
	pc_check((got[0] == 'H' && got[1] == 'C') || (got[0] == 'C' && got[1] == 'H'),
	    "the orphan got '%c' and '%c', expected SIGHUP and SIGCONT", got[0], got[1]);
	pc_pass();
}
