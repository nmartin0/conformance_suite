/*
 * A group orphaned by an exit, with a stopped member, gets SIGHUP and SIGCONT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.010
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
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

/* Reads one byte, waiting at most 10 s; 0 if none came. */
static int get1(int fd, char *c)
{
	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	if (poll(&pfd, 1, 10000) != 1)
		return 0;
	return read(fd, c, 1) == 1;
}

int main(void)
{
	int p[2], q[2], st, ok1, ok2;
	pid_t mid, g = -1;
	char got[2] = { 0, 0 };

	pc_begin("XSH._Exit.010");
	pc_check(pipe(p) == 0 && pipe(q) == 0, "pipe failed");
	pc_test();
	/*
	 * mid stays in our group; its child g makes a group of its own and
	 * stops. While mid lives, g's group has a parent (mid) in the session
	 * but outside the group; mid's exit removes it, which orphans the
	 * group.
	 */
	mid = spawn();
	if (mid == 0) {
		pid_t c;
		(void)close(p[0]);
		(void)close(q[0]);
		/* A new session: whoever adopts c after mid exits is outside it, so c's group is then orphaned. */
		if (setsid() == -1)
			_exit(95);
		c = fork();
		if (c == -1)
			_exit(97);
		if (c == 0) {
			struct sigaction sa;
			if (setpgid(0, 0) != 0)
				_exit(98);
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
		(void)setpgid(c, c);	/* also here, to close the race */
		(void)write(q[1], &c, sizeof c);
		{
			int s;
			if (waitpid(c, &s, WUNTRACED) != c || !WIFSTOPPED(s))
				_exit(96);
		}
		_exit(0);	/* orphans c's group */
	}
	(void)close(p[1]);
	(void)close(q[1]);
	if (read(q[0], &g, sizeof g) != (ssize_t)sizeof g)
		g = -1;
	st = reap(mid);
	if (g <= 1)
		pc_unresolved("the middle process did not report its child");
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0) {
		if (g > 1)
			(void)kill(-g, SIGKILL);
		pc_unresolved("the middle process could not set up (%#x)", st);
	}
	/*
	 * Both must arrive. The order in which pending signals reach their
	 * handlers is unspecified (XSH 2.4.2), so either order passes.
	 */
	ok1 = get1(p[0], &got[0]);
	ok2 = ok1 && get1(p[0], &got[1]);
	if (g > 1)
		(void)kill(-g, SIGKILL);	/* g's group is not ours: pcexec cannot */
	pc_check(ok1, "the stopped orphan got neither SIGHUP nor SIGCONT within 10 s");
	pc_check(ok2, "the stopped orphan got only one of SIGHUP and SIGCONT within 10 s");
	pc_check((got[0] == 'H' && got[1] == 'C') || (got[0] == 'C' && got[1] == 'H'),
	    "the orphan got '%c' and '%c', expected SIGHUP and SIGCONT", got[0], got[1]);
	pc_pass();
}
