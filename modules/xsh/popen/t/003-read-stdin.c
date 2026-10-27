/*
 * With "r" the command's standard input is the caller's.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.003
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
static void finish(FILE *f)
{
	if (pclose(f) == -1)
		pc_unresolved("pclose() failed: %s", pc_errname(errno));
}

/* Points descriptor fd at a file opened for reading; returns a saved copy of fd. */
static int redirect_in(int fd, const char *path)
{
	int saved = fcntl(fd, F_DUPFD_CLOEXEC, 10);
	int f = open(path, O_RDONLY);

	if (saved < 0 || f < 0 || dup2(f, fd) < 0)
		pc_unresolved("cannot redirect descriptor %d: %s", fd, pc_errname(errno));
	(void)close(f);
	return saved;
}
static void restore(int fd, int saved)
{
	(void)dup2(saved, fd);
	(void)close(saved);
}


int main(void)
{
	char line[64];
	int saved;
	FILE *f;

	pc_begin("XSH.popen.003");
	guard(20);
	pc_make_file("in", "in\n");
	pc_test();
	saved = redirect_in(0, "in");
	errno = 0;
	f = popen("cat", "r");
	restore(0, saved);
	pc_check(f != NULL, "popen() failed: %s", pc_errname(errno));
	pc_check(fgets(line, sizeof line, f) != NULL, "nothing could be read from the stream");
	pc_check(strcmp(line, "in\n") == 0, "the stream yielded \"%s\", expected the caller's input \"in\\n\"", line);
	finish(f);
	pc_pass();
}
