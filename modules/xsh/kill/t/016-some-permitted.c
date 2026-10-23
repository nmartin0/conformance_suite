/*
 * kill(0, sig) succeeds when only some group members may be signalled.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill.016
 * MUTANT    kill.eperm_pid0
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero. */
static int wait_for(volatile sig_atomic_t *flag, long ms)
{
	long i;

	for (i = 0; i < ms / 10 && !*flag; i++)
		sleep_ms(10);
	return *flag != 0;
}

static long ms_since(const struct timespec *t0)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long)(t.tv_sec - t0->tv_sec) * 1000 + (t.tv_nsec - t0->tv_nsec) / 1000000;
}

static int ready[2], hits[2], done[2];

static void ha(int s)
{
	(void)s;
	(void)write(hits[1], "A", 1);
}

static void hb(int s)
{
	(void)s;
	(void)write(hits[1], "B", 1);
}

/* A member: runs as uid, catches SIGUSR1, ends when done[] reaches EOF (or after 30 s). */
static pid_t member(uid_t uid, void (*fn)(int))
{
	pid_t p = fork();

	if (p == 0) {
		struct sigaction sa;
		char c;

		(void)close(done[1]);
		(void)alarm(30);
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = fn;
		sigemptyset(&sa.sa_mask);
		if (sigaction(SIGUSR1, &sa, NULL) != 0 || setgid(65534) != 0 || setuid(uid) != 0)
			_exit(99);
		(void)write(ready[1], "r", 1);
		while (read(done[0], &c, 1) != 0)
			;
		_exit(0);
	}
	return p;
}

/* The group leader (root): returns 0 when all went as required, 97 and up when it could not be set up. */
static int group(void)
{
	struct timespec t0;
	char buf[16], c;
	pid_t a, b, s;
	int st, code, got_a = 0, got_b = 0, i;
	ssize_t n;

	(void)signal(SIGUSR1, SIG_IGN);
	if (setpgid(0, 0) != 0)
		return 98;
	a = member(65534, ha);
	b = member(65533, hb);
	(void)close(ready[1]);
	(void)close(done[0]);
	code = a == -1 || b == -1 || read(ready[0], &c, 1) != 1 || read(ready[0], &c, 1) != 1 ? 99 : -1;
	if (code == -1) {
		s = fork();
		if (s == 0) {
			(void)signal(SIGUSR1, SIG_IGN);
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
			_exit(kill(0, SIGUSR1) == 0 ? 0 : errno == EPERM ? 2 : 3);
		}
		code = s == -1 || waitpid(s, &st, 0) != s || !WIFEXITED(st) ? 97 : WEXITSTATUS(st);
	}
	if (code == 0) {
		(void)clock_gettime(CLOCK_MONOTONIC, &t0);
		while (!got_a && ms_since(&t0) < 5000) {
			sleep_ms(10);
			while ((n = read(hits[0], buf, sizeof buf)) > 0)
				for (i = 0; i < n; i++)
					got_a |= buf[i] == 'A', got_b |= buf[i] == 'B';
		}
		sleep_ms(200);
		while ((n = read(hits[0], buf, sizeof buf)) > 0)
			for (i = 0; i < n; i++)
				got_b |= buf[i] == 'B';
		code = got_b ? 20 : got_a ? 0 : 21;
	}
	(void)close(done[1]);	/* the members see EOF and end */
	if (a > 0)
		(void)waitpid(a, &st, 0);
	if (b > 0)
		(void)waitpid(b, &st, 0);
	return code;
}

int main(void)
{
	pc_begin("XSH.kill.016");
	{
		pid_t g;
		int st;

		if (geteuid() != 0)
			pc_unresolved("needs root to run processes of two users");
		pc_check(pipe(ready) == 0 && pipe(hits) == 0 && pipe(done) == 0, "pipe failed");
		pc_check(fcntl(hits[0], F_SETFL, O_NONBLOCK) == 0, "fcntl failed");
		pc_test();
		g = fork();
		if (g == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (g == 0)
			_exit(group());
		while (waitpid(g, &st, 0) == -1 && errno == EINTR)
			;
		pc_check(WIFEXITED(st), "the group leader did not exit (status %#x)", (unsigned)st);
		if (WEXITSTATUS(st) >= 97)
			pc_unresolved("the group could not be set up (%d): users 65534 and 65533 needed", WEXITSTATUS(st));
		pc_check(WEXITSTATUS(st) != 2, "kill(0, SIGUSR1) failed with EPERM though one target was permitted");
		pc_check(WEXITSTATUS(st) != 3, "kill(0, SIGUSR1) failed with another error");
		pc_check(WEXITSTATUS(st) != 20, "the process of another user got the signal");
		pc_check(WEXITSTATUS(st) != 21, "the permitted process did not get the signal");
		pc_check(WEXITSTATUS(st) == 0, "the group leader exited with %d", WEXITSTATUS(st));
	}
	pc_pass();
}
