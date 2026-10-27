/*
 * getlogin() returns null or a well-formed name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlogin.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* A call that blocks must fail the test, not hang it. */
static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: the call under test did not finish in time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

/* 0 if p is null or a non-empty string within {LOGIN_NAME_MAX}; 1 if empty; 2 if too long. */
static int form(const char *p)
{
	long max = sysconf(_SC_LOGIN_NAME_MAX);
	size_t n;

	if (p == NULL)
		return 0;
	n = strlen(p);
	if (n == 0)
		return 1;
	if (max > 0 && n > (size_t)max)
		return 2;
	return 0;
}

int main(void)
{
	pid_t c;
	int f, st;

	pc_begin("XSH.getlogin.001");
	guard(20);
	pc_test();
	f = form(getlogin());
	pc_check(f != 1, "getlogin() returned an empty string");
	pc_check(f != 2, "getlogin() returned a name longer than {LOGIN_NAME_MAX}");
	/* again without a controlling terminal, where a null pointer is the likely answer */
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		if (setsid() == -1)
			_exit(90);
		_exit(form(getlogin()));
	}
	if (waitpid(c, &st, 0) != c)
		pc_unresolved("waitpid failed: %s", pc_errname(errno));
	pc_check(WIFEXITED(st), "the child ended abnormally (status %#x)", st);
	if (WEXITSTATUS(st) == 90)
		pc_unresolved("setsid() failed in the child");
	pc_check(WEXITSTATUS(st) != 1, "getlogin() returned an empty string without a controlling terminal");
	pc_check(WEXITSTATUS(st) != 2, "getlogin() returned a name longer than {LOGIN_NAME_MAX} without a controlling terminal");
	pc_pass();
}
