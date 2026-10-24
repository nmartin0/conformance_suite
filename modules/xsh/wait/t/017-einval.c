/*
 * An invalid options value gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.017
 * MUTANT    wait.einval_ok
 * VARIANT   wait.ext_option
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t fired;

static void on_alarm(int sig)
{
	(void)sig;
	fired = 1;
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

/* A child that does nothing but sleep for ms milliseconds, then _exit(code). */
static pid_t child(long ms, int code)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		struct timespec ts;
		ts.tv_sec = ms / 1000;
		ts.tv_nsec = (ms % 1000) * 1000000L;
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
		_exit(code);
	}
	return r;
}

static long now_ms(void)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}

/*
 * An implementation may define option flags beyond the standard ones (the
 * W prefix is reserved in <sys/wait.h>, XSH 2.2.2), so no one bit is sure
 * to be invalid. The test tries bit 22 (0x400000), which no known system
 * defines, then every other bit from 4 to 30 that the header does not
 * name: an extension may accept a bit (and may give it a meaning that
 * leads to another result or error), but options that use only bits no
 * flag on this system uses are not valid, and at least one of these must
 * fail with EINVAL.
 */
#define FIRSTBIT 0x400000

static int named(long bit)
{
	long known = WNOHANG | WUNTRACED | WEXITED | WSTOPPED | WNOWAIT;
#ifdef WCONTINUED
	known |= WCONTINUED;
#endif
	return (bit & known) != 0;
}

int main(void)
{
	pid_t c;
	int st, i, tried = 0;
	long bit, r;
	char seen[256];
	size_t n = 0;

	pc_begin("XSH.wait.017");
	c = child(5000, 0);
	seen[0] = '\0';
	pc_test();
	for (i = 3; i <= 30; i++) {
		int e;

		bit = i == 3 ? FIRSTBIT : 1L << i;
		if ((i > 3 && bit == FIRSTBIT) || named(bit))
			continue;
		tried++;
		errno = 0;
		r = waitpid(c, &st, WNOHANG | (int)bit);
		e = errno;
		if (r == -1 && e == EINVAL)
			break;
		if (r == c)
			pc_unresolved("waitpid() with option bit %#lx reported the child, which was still running", bit);
		if (n < sizeof seen - 40)
			n += (size_t)snprintf(seen + n, sizeof seen - n, " %#lx:%s", bit,
			    r == -1 ? pc_errname(e) : "0");
	}
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
	pc_check(i <= 30, "waitpid() accepted all %d option bits tried that no flag uses (bit:result%s)",
	    tried, seen);
	pc_pass();
}
