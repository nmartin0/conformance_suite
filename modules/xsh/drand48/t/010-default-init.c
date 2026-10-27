/*
 * drand48(), lrand48() and mrand48() work without initialisation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.drand48.010
 */
#include "pc.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

struct out {
	double d[4];
	long l[4];
	long m[4];
};

static void on_alarm(int sig)
{
	(void)sig;
	pc_fail("a child did not finish in time");
}

static void child(int fd)
{
	struct out o;
	int i;
	ssize_t w;

	for (i = 0; i < 4; i++)
		o.d[i] = drand48();
	for (i = 0; i < 4; i++)
		o.l[i] = lrand48();
	for (i = 0; i < 4; i++)
		o.m[i] = mrand48();
	w = write(fd, &o, sizeof o);
	_exit(w == (ssize_t)sizeof o ? 0 : 1);
}

int main(void)
{
	struct out o[2];
	int run, i;
	struct sigaction sa;

	pc_begin("XSH.drand48.010");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(30);
	pc_test();
	for (run = 0; run < 2; run++) {
		int p[2], st;
		pid_t c;
		ssize_t n;

		if (pipe(p) != 0)
			pc_unresolved("pipe failed");
		c = fork();
		if (c == -1)
			pc_unresolved("fork failed");
		if (c == 0) {
			(void)close(p[0]);
			child(p[1]);
		}
		(void)close(p[1]);
		n = read(p[0], &o[run], sizeof o[run]);
		(void)close(p[0]);
		while (waitpid(c, &st, 0) == -1)
			;
		pc_check(n == (ssize_t)sizeof o[run], "a child reported %ld bytes", (long)n);
		pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "a child failed (status %#x)", st);
	}
	for (i = 0; i < 4; i++) {
		pc_check(o[0].d[i] >= 0.0 && o[0].d[i] < 1.0, "drand48() without initialisation returned %.17g", o[0].d[i]);
		pc_check(o[0].l[i] >= 0 && o[0].l[i] <= 2147483647L, "lrand48() without initialisation returned %ld", o[0].l[i]);
		pc_check(o[0].m[i] >= -2147483647L - 1 && o[0].m[i] <= 2147483647L, "mrand48() without initialisation returned %ld", o[0].m[i]);
	}
	pc_check(memcmp(&o[0], &o[1], sizeof o[0]) == 0, "two fresh processes got different default sequences");
	pc_pass();
}
