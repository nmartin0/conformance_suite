/*
 * The CPU-time clock of another process reads at least what that process
 * had read from its own clock.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getcpuclockid.003
 */
#include "cpuh.h"
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	int a[2], b[2], e, st;
	pid_t p;
	clockid_t id;
	char buf[64];
	long long mine, theirs;
	struct pollfd pf;
	ssize_t n;

	pc_begin("XSH.clock_getcpuclockid.003");
	if (pipe(a) != 0 || pipe(b) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	p = fork();
	if (p < 0)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (p == 0) {
		clockid_t me;
		char c;

		alarm(30);
		close(a[0]);
		close(b[1]);
		if (clock_getcpuclockid(0, &me) != 0 || cpu_burn(me) == 0)
			snprintf(buf, sizeof buf, "x");
		else
			snprintf(buf, sizeof buf, "%lld", cpu_ns(me));
		n = write(a[1], buf, strlen(buf) + 1);
		(void)n;
		n = read(b[0], &c, 1);
		(void)n;
		_exit(0);
	}
	close(a[1]);
	close(b[0]);
	pf.fd = a[0];
	pf.events = POLLIN;
	buf[0] = 0;
	if (poll(&pf, 1, 10000) <= 0 || (n = read(a[0], buf, sizeof buf - 1)) <= 0)
		buf[0] = 'x';
	buf[sizeof buf - 1] = 0;
	if (buf[0] == 'x' || (mine = atoll(buf)) <= 0) {
		kill(p, SIGKILL);
		waitpid(p, &st, 0);
		pc_unresolved("the child could not use its own CPU-time clock");
	}
	pc_test();
	e = clock_getcpuclockid(p, &id);
	if (e == EPERM) {
		kill(p, SIGKILL);
		waitpid(p, &st, 0);
		pc_unresolved("permission to read the child's CPU-time clock was refused");
	}
	if (e != 0) {
		kill(p, SIGKILL);
		waitpid(p, &st, 0);
		pc_fail("clock_getcpuclockid(child) failed: %s", pc_errname(e));
	}
	theirs = cpu_ns(id);
	n = write(b[1], "g", 1);
	(void)n;
	waitpid(p, &st, 0);
	pc_check(theirs >= 0, "clock_gettime on the child's clock failed");
	pc_check(theirs >= mine, "the clock reads %lld ns, below the %lld ns the child had read from it", theirs, mine);
	pc_pass();
}
