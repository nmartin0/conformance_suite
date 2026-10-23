/*
 * killpg() without permission for any member gives EPERM.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.killpg.004
 * MUTANT    killpg.eperm_hidden
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

int main(void)
{
	pc_begin("XSH.killpg.004");
	{
		pid_t l, s;
		int p[2], st;
		char c;

		if (geteuid() != 0)
			pc_unresolved("needs root to run a process of another user");
		pc_check(pipe(p) == 0, "pipe failed");
		l = fork();
		if (l == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (l == 0) {
			(void)setpgid(0, 0);
			(void)write(p[1], "r", 1);
			for (;;)
				pause();
		}
		pc_check(read(p[0], &c, 1) == 1, "the group leader did not start");
		pc_test();
		s = fork();
		if (s == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (s == 0) {
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
			_exit(killpg(l, SIGUSR1) == 0 ? 1 : errno == EPERM ? 0 : 2);
		}
		pc_check(waitpid(s, &st, 0) == s && WIFEXITED(st), "the sender did not exit");
		sleep_ms(100);
		{
			int st2;
			pid_t r = waitpid(l, &st2, WNOHANG);
			(void)kill(l, SIGKILL);
			(void)waitpid(l, NULL, 0);
			if (WEXITSTATUS(st) == 99)
				pc_unresolved("the sender could not take user 65534");
			pc_check(r == 0, "the group's member was ended by the signal");
		}
		pc_check(WEXITSTATUS(st) != 1, "killpg() without permission succeeded");
		pc_check(WEXITSTATUS(st) == 0, "killpg() without permission failed with an error other than EPERM");
	}
	pc_pass();
}
