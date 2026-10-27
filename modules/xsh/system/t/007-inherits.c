/*
 * The command has the caller's directory, environment and descriptors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.007
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

static void restore(int fd, int saved)
{
	(void)dup2(saved, fd);
	(void)close(saved);
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

int main(void)
{
	int r, s1, s2;
	char out[256], err[256];

	pc_begin("XSH.system.007");
	pc_make_file("marker", "x");
	pc_check(setenv("PC_ENV_TEST", "abc", 1) == 0, "setenv failed");
	pc_test();
	s1 = redirect(1, "out");
	s2 = redirect(2, "err");
	r = system("test -f marker && test \"$PC_ENV_TEST\" = abc && echo OUT && echo ERR >&2");
	restore(1, s1);
	restore(2, s2);
	(void)snprintf(out, sizeof out, "%s", slurp_file("out"));
	(void)snprintf(err, sizeof err, "%s", slurp_file("err"));
	pc_check(r != -1, "system() failed: %s", pc_errname(errno));
	pc_check(WIFEXITED(r) && WEXITSTATUS(r) == 0,
	    "status %#x: the command did not find the caller's file in the working directory and its environment variable", r);
	pc_check(strcmp(out, "OUT\n") == 0, "the caller's standard output file holds \"%s\", expected \"OUT\\n\"", out);
	pc_check(strcmp(err, "ERR\n") == 0, "the caller's standard error file holds \"%s\", expected \"ERR\\n\"", err);
	pc_pass();
}
