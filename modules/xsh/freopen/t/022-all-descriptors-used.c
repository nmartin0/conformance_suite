/*
 * freopen() closes the old descriptor first, so it succeeds with every descriptor in use.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.freopen.022
 * MUTANT    freopen.opens_first
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

static int ts_after(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec);
}

/*
 * Set the error indicator of a stream that has nothing buffered for input:
 * its descriptor is closed for one fgetc(), whose read fails with EBADF
 * (XSH fgetc(): data needs to be read), then put back with dup2(). The
 * copy is never used to affect the offset, so it is not a second handle
 * (XSH 2.5.1).
 */
static void set_error(FILE *f)
{
	int fd = fileno(f), d = dup(fd);

	NEED(fd >= 0 && d >= 0, "dup failed: %s", pc_errname(errno));
	NEED(close(fd) == 0, "close failed");
	NEED(fgetc(f) == EOF, "fgetc() on a closed descriptor returned a byte");
	NEED(dup2(d, fd) == fd && close(d) == 0, "dup2 failed: %s", pc_errname(errno));
	NEED(ferror(f), "fgetc() on a closed descriptor did not set the error indicator");
}

#define LIM 32

/* 0: freopen() worked; 1: it failed; 2: wrong data; 9x: setup failed. */
static int child(void)
{
	struct rlimit rl;
	FILE *f, *r;
	int k;

	f = fopen("a", "r");
	if (f == NULL || getrlimit(RLIMIT_NOFILE, &rl) != 0)
		return 99;
	if (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < LIM)
		return 98;
	for (k = LIM; k < 1024 && (rl.rlim_cur == RLIM_INFINITY || (rlim_t)k < rl.rlim_cur); k++)
		if (fcntl(k, F_GETFD) != -1)
			return 98;	/* a descriptor at or above the limit is open */
	rl.rlim_cur = LIM;
	if (setrlimit(RLIMIT_NOFILE, &rl) != 0)
		return 98;
	while (open("a", O_RDONLY) >= 0)
		;
	if (errno != EMFILE)
		return 97;
	if (open("a", O_RDONLY) != -1)
		return 97;
	r = freopen("b", "r", f);
	if (r != f)
		return 1;
	return fgetc(f) == 'b' ? 0 : 2;
}

int main(void)
{
	pc_begin("XSH.freopen.022");
	{
		pid_t kid;
		int st;

		pc_make_file("a", "AAAA");
		pc_make_file("b", "bcd");
		pc_test();
		kid = fork();
		NEED(kid != -1, "fork failed");
		if (kid == 0)
			_exit(child());
		NEED(waitpid(kid, &st, 0) == kid, "waitpid failed");
		NEED(WIFEXITED(st) && WEXITSTATUS(st) < 90, "the child could not fill its descriptors (status %#x)",
		    (unsigned)st);
		pc_check(WEXITSTATUS(st) != 1, "with every descriptor in use, freopen() failed (it must close the old one first)");
		pc_check(WEXITSTATUS(st) == 0, "after freopen() the stream did not read the new file");
	}
	pc_pass();
}
