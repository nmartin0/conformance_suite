/*
 * freopen() without the permissions the mode needs fails with EACCES.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.freopen.021
 * MUTANT    freopen.wrong_errno
 * MUTANT    freopen.returns_stream
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

/*
 * Setup for unprivileged(): when run as root, make the working directory
 * searchable for the child (relative paths need nothing above it), and
 * leave a file no one may read, which the child can open only with
 * privileges.
 */
static void pc_searchable(void)
{
	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	pc_make_file(".pc-priv", "");
	if (chmod(".pc-priv", 0) != 0)
		pc_unresolved("chmod(.pc-priv) failed: %s", pc_errname(errno));
}

/* After dropping privileges: no supplementary group but the child's own. */
static int groups_dropped(void)
{
	gid_t gs[64];
	int n = getgroups(64, gs), i;

	if (n < 0)
		return 0;
	for (i = 0; i < n; i++)
		if (gs[i] != getegid() && gs[i] != getgid())
			return 0;
	return 1;
}

/*
 * Run fn in a child that has no appropriate privileges: when the test runs
 * as root, the child takes user and group 65534. Returns what fn returns
 * (0 to 96); a child that cannot drop privileges, that still has them (it
 * can read the mode 0 file .pc-priv), or that dies, makes the test
 * UNRESOLVED.
 */
static int unprivileged(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (geteuid() == 0) {
			(void)pc_drop_groups();
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		if (!groups_dropped())
			_exit(98);
		if (open(".pc-priv", O_RDONLY) >= 0)
			_exit(97);
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (WIFEXITED(st) && WEXITSTATUS(st) == 97)
		pc_unresolved("the child still has appropriate privileges (it can read a mode 0 file)");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 98)
		pc_unresolved("the unprivileged child kept supplementary groups (no way to drop them here)");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

/* 0: NULL with EACCES; 1: it succeeded; 2: another errno; 90: setup failed. */
static int try(const char *path, const char *mode)
{
	FILE *f = fopen("s", "r"), *r;

	if (f == NULL)
		return 90;
	errno = 0;
	r = freopen(path, mode, f);
	if (r != NULL)
		return 1;
	return errno == EACCES ? 0 : 2;
}

static int child(void)
{
	int r;

	if ((r = try("r0222", "r")) != 0)
		return 10 + r;
	if ((r = try("w0444", "w")) != 0)
		return 20 + r;
	if ((r = try("ro/new", "w")) != 0)
		return 30 + r;
	return 0;
}

int main(void)
{
	pc_begin("XSH.freopen.021");
	{
		static const char *what[] = { "\"r\" on a mode 0222 file", "\"w\" on a mode 0444 file",
			"\"w\" for a new file in a mode 0555 directory" };
		int r, w;

		pc_searchable();
		pc_make_file("s", "s");
		pc_make_file("r0222", "r");
		pc_make_file("w0444", "w");
		pc_make_dir("ro");
		if (!(chmod("s", 0644) == 0 && chmod("r0222", 0222) == 0 && chmod("w0444", 0444) == 0 &&
		    chmod("ro", 0555) == 0))
			pc_unresolved("chmod failed");
		pc_test();
		r = unprivileged(child);
		NEED(r == 0 || (r >= 11 && r <= 32 && r % 10 != 0), "the child could not open its stream (status %d)", r);
		w = r == 0 ? 0 : r / 10 - 1;
		pc_check(r == 0 || r % 10 != 1, "freopen() with %s succeeded without permission", what[w]);
		pc_check(r == 0, "freopen() with %s failed with an errno other than EACCES", what[w]);
		pc_check(access("ro/new", F_OK) == -1, "a file was created in the mode 0555 directory");
	}
	pc_pass();
}
