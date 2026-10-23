/*
 * fseek(), fsetpos(), rewind() and fflush() discard pushed-back bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ungetc.003
 * MUTANT    ungetc.survives_seek
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static int same_file(const struct stat *a, const struct stat *b)
{
	return a->st_dev == b->st_dev && a->st_ino == b->st_ino;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static int ts_cmp(struct timespec a, struct timespec b)
{
	if (a.tv_sec != b.tv_sec)
		return a.tv_sec < b.tv_sec ? -1 : 1;
	if (a.tv_nsec != b.tv_nsec)
		return a.tv_nsec < b.tv_nsec ? -1 : 1;
	return 0;
}

#ifndef PATH_MAX
#define PATH_MAX 4096	/* <limits.h> may omit it; tests only need a big buffer */
#endif

/*
 * Setup for unprivileged(): when run as root, make the working directory
 * searchable for the child (relative paths need nothing above it). It is
 * called right after pc_begin(), in the setup phase, so no planted fault
 * applies to it.
 */
static void pc_searchable(void)
{
	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	/* A file no one may read: the child can open it only with privileges. */
	pc_make_file(".pc-priv", "");
	if (chmod(".pc-priv", 0) != 0)
		pc_unresolved("chmod(.pc-priv) failed: %s", pc_errname(errno));
}

/*
 * Run fn in a child that has no appropriate privileges: when the test runs
 * as root, the child takes user and group 65534 (the working directory is
 * made searchable for it; relative paths need nothing above it). Returns
 * what fn returns (0 to 96); a child that cannot drop privileges, that
 * still has them (it can read the mode 0 file .pc-priv, as with a
 * capability), or that dies, makes the test UNRESOLVED.
 */
static int unprivileged(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (geteuid() == 0 && (setgid(65534) != 0 || setuid(65534) != 0))
			_exit(99);
		if (geteuid() == 0)
			_exit(99);
		{
			int pfd = open(".pc-priv", O_RDONLY);
			if (pfd >= 0)
				_exit(97);
		}
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (WIFEXITED(st) && WEXITSTATUS(st) == 97)
		pc_unresolved("the child still has appropriate privileges (it can read a mode 0 file)");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

/* The status of path from a descriptor of it (setup: no mutants apply). */
static struct stat via_fd(const char *path, int flags)
{
	struct stat st;
	int fd = open(path, flags);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	pc_check(fstat(fd, &st) == 0, "fstat(%s) failed: %s", path, pc_errname(errno));
	(void)close(fd);
	return st;
}

static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

/* A stream reading path (created in setup with s). */
static FILE *reader(const char *path)
{
	FILE *f = fopen(path, "r");

	pc_check(f != NULL, "fopen(%s) failed: %s", path, pc_errname(errno));
	return f;
}

int main(void)
{
	pc_begin("XSH.ungetc.003");
	{
		fpos_t start;
		FILE *f;
		int r;

		pc_make_file("f", "0123");
		pc_test();
		f = reader("f");
		pc_check(fgetpos(f, &start) == 0, "fgetpos failed");
		pc_check(fgetc(f) == '0' && ungetc('Z', f) == 'Z', "fgetc or ungetc failed");
		pc_check(fseek(f, 1, SEEK_SET) == 0, "fseek failed");
		r = fgetc(f);
		pc_check(r == '1', "after fseek() the read gave %#x, expected '1'", (unsigned)r);
		pc_check(ungetc('Z', f) == 'Z' && fsetpos(f, &start) == 0, "ungetc or fsetpos failed");
		r = fgetc(f);
		pc_check(r == '0', "after fsetpos() the read gave %#x, expected '0'", (unsigned)r);
		pc_check(ungetc('Z', f) == 'Z', "ungetc failed");
		rewind(f);
		r = fgetc(f);
		pc_check(r == '0', "after rewind() the read gave %#x, expected '0'", (unsigned)r);
		pc_check(ungetc('Z', f) == 'Z' && fflush(f) == 0, "ungetc or fflush failed");
		r = fgetc(f);
		pc_check(r != 'Z', "after fflush() the discarded 'Z' was read");
	}
	pc_pass();
}
