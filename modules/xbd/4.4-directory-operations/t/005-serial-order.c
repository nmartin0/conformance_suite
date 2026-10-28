/*
 * Directory operations of one process are seen by another in the order they
 * were done: names created one after another are never seen with a gap, and
 * names removed one after another are never seen to vanish out of order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.4-directory-operations.005
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define N 300

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

static void nap(long us)
{
	struct timespec ts;

	ts.tv_sec = 0;
	ts.tv_nsec = us * 1000L;
	(void)nanosleep(&ts, NULL);
}

static void fname(char *b, size_t n, int i)
{
	(void)snprintf(b, n, "f%03d", i);
}

static int exists(const char *p)
{
	struct stat sb;

	return lstat(p, &sb) == 0;
}

/*
 * Observer. phase 0: the other process creates f000, f001, ... one at a
 * time; a scan from the highest index down that finds a name must then find
 * every lower one. phase 1: the other process removes f000, f001, ...; a
 * scan from the highest down that misses a name must then miss every lower
 * one. Exit: 0 consistent and some scan saw a change under way, 1 a
 * violation, 2 consistent but no scan caught the change under way.
 */
static int observer(int phase)
{
	char nm[16];
	int i, state, mixed = 0;

	(void)signal(SIGALRM, SIG_DFL);
	(void)alarm(100);
	for (;;) {
		int seen_hi = 0, prev = -1, bad = 0, fin;

		for (i = N - 1; i >= 0; i--) {
			fname(nm, sizeof nm, i);
			state = exists(nm);
			if (phase == 0) {
				if (prev == 1 && !state)
					bad = 1;
				if (state)
					seen_hi = 1;
				if (!state && seen_hi)
					bad = 1;
			} else {
				if (prev == 0 && state)
					bad = 1;
			}
			prev = state;
		}
		if (bad)
			return 1;
		/* a scan holding both present and absent names saw the operations part way */
		{
			int any = 0, none = 0;

			for (i = 0; i < N; i += 7) {
				fname(nm, sizeof nm, i);
				if (exists(nm))
					any = 1;
				else
					none = 1;
			}
			if (any && none)
				mixed++;
		}
		fin = exists(phase == 0 ? "created" : "removed");
		if (fin)
			break;
	}
	return mixed > 0 ? 0 : 2;
}

int main(void)
{
	int phase, i, st, fd;
	pid_t kid;
	char nm[16];

	pc_begin("XBD.4.4-directory-operations.005");
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(120);
	for (phase = 0; phase < 2; phase++) {
		if (phase == 1) {
			for (i = 0; i < N; i++) {
				fname(nm, sizeof nm, i);
				if (!exists(nm))
					pc_unresolved("%s is missing before the removal phase", nm);
			}
		}
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0)
			_exit(observer(phase));
		nap(20000);	/* let the observer start scanning */
		for (i = 0; i < N; i++) {
			fname(nm, sizeof nm, i);
			if (phase == 0) {
				fd = open(nm, O_WRONLY | O_CREAT | O_EXCL, 0600);
				if (fd < 0)
					pc_unresolved("creating %s failed: %s", nm, pc_errname(errno));
				(void)close(fd);
			} else if (unlink(nm) != 0) {
				pc_unresolved("removing %s failed: %s", nm, pc_errname(errno));
			}
			nap(300);
		}
		nap(20000);
		fd = open(phase == 0 ? "created" : "removed", O_WRONLY | O_CREAT, 0600);
		if (fd < 0)
			pc_unresolved("creating the end marker failed: %s", pc_errname(errno));
		(void)close(fd);
		if (waitpid(kid, &st, 0) != kid || !WIFEXITED(st))
			pc_unresolved("the observer did not end normally");
		pc_check(WEXITSTATUS(st) != 1, "the observer saw the %s out of order", phase == 0 ? "creations" : "removals");
		if (WEXITSTATUS(st) == 2)
			pc_unresolved("the observer never caught the %s under way, so the order was not exercised", phase == 0 ? "creations" : "removals");
		pc_check(WEXITSTATUS(st) == 0, "the observer ended with code %d", WEXITSTATUS(st));
	}
	pc_pass();
}
