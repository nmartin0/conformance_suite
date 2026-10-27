/*
 * With "w" the command's standard output and error are the caller's.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.004
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

/* Points descriptor fd at a new file; returns a saved copy of fd. */
static int redirect(int fd, const char *path)
{
	int saved = fcntl(fd, F_DUPFD_CLOEXEC, 10);
	int f = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (saved < 0 || f < 0 || dup2(f, fd) < 0)
		pc_unresolved("cannot redirect descriptor %d: %s", fd, pc_errname(errno));
	(void)close(f);
	return saved;
}

/* The file's contents (up to 255 bytes) as a string; "" if it cannot be read. */
static const char *slurp_file(const char *path)
{
	static char b[256];
	int f = open(path, O_RDONLY);
	ssize_t n;

	b[0] = '\0';
	if (f < 0)
		return b;
	n = read(f, b, sizeof b - 1);
	(void)close(f);
	if (n > 0)
		b[n] = '\0';
	else
		b[0] = '\0';
	return b;
}
static void restore(int fd, int saved)
{
	(void)dup2(saved, fd);
	(void)close(saved);
}


int main(void)
{
	int s1, s2;
	FILE *f;

	pc_begin("XSH.popen.004");
	guard(20);
	pc_test();
	s1 = redirect(1, "out");
	s2 = redirect(2, "err");
	errno = 0;
	f = popen("cat; echo ERR >&2", "w");
	if (f != NULL) {
		(void)fputs("hello\n", f);
		(void)pclose(f);
	}
	restore(1, s1);
	restore(2, s2);
	pc_check(f != NULL, "popen() failed: %s", pc_errname(errno));
	pc_check(strcmp(slurp_file("out"), "hello\n") == 0, "the caller's standard output file holds \"%s\", expected \"hello\\n\"", slurp_file("out"));
	pc_check(strcmp(slurp_file("err"), "ERR\n") == 0, "the caller's standard error file holds \"%s\", expected \"ERR\\n\"", slurp_file("err"));
	pc_pass();
}
