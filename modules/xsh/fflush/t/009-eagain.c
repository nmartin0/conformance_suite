/*
 * fflush() to a full non-blocking pipe gives EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fflush.009
 * MUTANT    fflush.errno_lost
 * MUTANT    fflush.success_on_error
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/time.h>
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

/* A new file f holding s (setup). */
static void make(const char *path, const char *s)
{
	pc_make_file(path, s);
}

/* The file's contents (at most size - 1 bytes). */
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

static off_t offset(int fd)
{
	return lseek(fd, 0, SEEK_CUR);
}

/* Gives f a fully buffered 16 KiB buffer of the test's own (sizes passed
   with a null buffer may be ignored). */
static void full(FILE *f)
{
	char *b = malloc(16384);

	pc_check(f != NULL && b != NULL, "no stream or no memory");
	pc_check(setvbuf(f, b, _IOFBF, 16384) == 0, "setvbuf failed");
}

/* A stream on path in mode, fully buffered. */
static FILE *open_full(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	pc_check(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	full(f);
	return f;
}

/* Sets the file's st_mtim and st_atim to 2001-09-09 through fd. */
static void old_times(int fd)
{
	struct timespec v[2];

	v[0].tv_sec = v[1].tv_sec = 1000000000;
	v[0].tv_nsec = v[1].tv_nsec = 0;
	pc_check(futimens(fd, v) == 0, "futimens failed: %s", pc_errname(errno));
}

static int sigpipe_pending(void)
{
	sigset_t p;

	return sigpending(&p) == 0 && sigismember(&p, SIGPIPE) == 1;
}

static void block_sigpipe(void)
{
	sigset_t one;

	sigemptyset(&one);
	sigaddset(&one, SIGPIPE);
	pc_check(sigprocmask(SIG_BLOCK, &one, NULL) == 0, "sigprocmask failed");
}

int main(void)
{
	pc_begin("XSH.fflush.009");
	{
		static char blk[4096];
		FILE *f;
		int p[2], r, e;

		pc_check(pipe(p) == 0, "pipe failed");
		pc_check(fcntl(p[1], F_SETFL, O_NONBLOCK) == 0, "fcntl failed");
		memset(blk, 'z', sizeof blk);
		while (write(p[1], blk, sizeof blk) > 0)
			;
		pc_check(errno == EAGAIN, "filling the pipe failed: %s", pc_errname(errno));
		pc_test();
		f = fdopen(p[1], "w");
		pc_check(f != NULL, "fdopen failed");
		full(f);
		pc_check(fwrite(blk, 1, sizeof blk, f) == sizeof blk, "fwrite failed");
		errno = 0;
		r = fflush(f);
		e = errno;
		pc_check(r == EOF, "fflush() returned %d, expected EOF", r);
		pc_check(e == EAGAIN, "fflush() set errno %s, expected EAGAIN", pc_errname(e));
	}
	pc_pass();
}
