/*
 * kill(0, sig) succeeds when only some group members may be signalled.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill.016
 * MUTANT    kill.eperm_if_any
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

static int ready[2], hits[2];

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

static pid_t member(uid_t uid, void (*fn)(int))
{
	pid_t p = fork();

	if (p == 0) {
		struct sigaction sa;
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = fn;
		sigemptyset(&sa.sa_mask);
		if (sigaction(SIGUSR1, &sa, NULL) != 0 || setgid(65534) != 0 || setuid(uid) != 0)
			_exit(99);
		(void)write(ready[1], "r", 1);
		for (;;)
			pause();
	}
	return p;
}

/* The group leader (root): returns 0 when all went as required. */
static int group(void)
{
	char buf[16];
	pid_t a, b, s;
	ssize_t n;
	int st, code, i;

	(void)signal(SIGUSR1, SIG_IGN);
	if (setpgid(0, 0) != 0)
		return 98;
	a = member(65534, ha);
	b = member(65533, hb);
	if (a == -1 || b == -1 || read(ready[0], buf, 1) != 1 || read(ready[0], buf, 1) != 1)
		return 99;
	s = fork();
	if (s == 0) {
		(void)signal(SIGUSR1, SIG_IGN);
		if (setgid(65534) != 0 || setuid(65534) != 0)
			_exit(99);
		_exit(kill(0, SIGUSR1) == 0 ? 0 : errno == EPERM ? 2 : 3);
	}
	if (s == -1 || waitpid(s, &st, 0) != s || !WIFEXITED(st))
		return 97;
	code = WEXITSTATUS(st);
	sleep_ms(300);
	n = read(hits[0], buf, sizeof buf);
	(void)kill(a, SIGKILL);
	(void)kill(b, SIGKILL);
	(void)waitpid(a, &st, 0);
	(void)waitpid(b, &st, 0);
	if (code != 0)
		return code;
	for (i = 0; i < n; i++)
		if (buf[i] == 'B')
			return 20;
	return n >= 1 && buf[0] == 'A' ? 0 : 21;
}

int main(void)
{
	pc_begin("XSH.kill.016");
	{
		pid_t g;
		int st;

		if (geteuid() != 0)
			pc_unresolved("needs root to run processes of two users");
		pc_check(pipe(ready) == 0 && pipe(hits) == 0, "pipe failed");
		pc_check(fcntl(hits[0], F_SETFL, O_NONBLOCK) == 0, "fcntl failed");
		pc_test();
		g = fork();
		if (g == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (g == 0)
			_exit(group());
		pc_check(waitpid(g, &st, 0) == g && WIFEXITED(st), "the group leader did not exit");
		if (WEXITSTATUS(st) == 99)
			pc_unresolved("the processes could not take users 65534 and 65533");
		pc_check(WEXITSTATUS(st) != 2, "kill(0, SIGUSR1) failed with EPERM though one target was permitted");
		pc_check(WEXITSTATUS(st) != 3, "kill(0, SIGUSR1) failed with another error");
		pc_check(WEXITSTATUS(st) != 20, "the process of another user got the signal");
		pc_check(WEXITSTATUS(st) != 21, "the permitted process did not get the signal");
		pc_check(WEXITSTATUS(st) == 0, "the group leader exited with %d", WEXITSTATUS(st));
	}
	pc_pass();
}
