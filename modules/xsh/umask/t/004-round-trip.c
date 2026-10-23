/*
 * umask(umask(x)) restores the mask exactly.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.umask.004
 * MUTANT    umask.returns_new
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

static struct stat lst(const char *path)
{
	struct stat st;

	pc_check(lstat(path, &st) == 0, "lstat(%s) failed: %s", path, pc_errname(errno));
	return st;
}

static void long_name(char *buf, size_t size)
{
	long max = pathconf(".", _PC_NAME_MAX);

	if (max < 0 || max + 2 > (long)size)
		pc_unresolved("{NAME_MAX} is indeterminate or too large here (%ld)", max);
	memset(buf, 'n', (size_t)max + 1);
	buf[max + 1] = '\0';
}

/* 0: failed with one of the expected errors; 1: succeeded; 10 + errno: another errno. */
static int verdict(int r, int e1, int e2)
{
	if (r == 0)
		return 1;
	return errno == e1 || errno == e2 ? 0 : 10 + (errno < 86 ? errno : 86);
}

/* The file's whole contents (at most size - 1 bytes) and its length. */
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

/*
 * Run fn in a child whose soft RLIMIT_FSIZE is LIMIT bytes, with SIGXFSZ
 * blocked (where it exists) so that exceeding the limit cannot kill it.
 * Returns what fn returns (0 to 96); a child that cannot set the limit
 * makes the test UNRESOLVED.
 */
#define LIMIT 4096

static int limited(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		struct rlimit rl;
#ifdef SIGXFSZ
		sigset_t set;

		sigemptyset(&set);
		sigaddset(&set, SIGXFSZ);
		if (sigprocmask(SIG_BLOCK, &set, NULL) != 0)
			_exit(98);
#endif
		if (getrlimit(RLIMIT_FSIZE, &rl) != 0 || (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < LIMIT))
			_exit(98);
		rl.rlim_cur = LIMIT;
		if (setrlimit(RLIMIT_FSIZE, &rl) != 0)
			_exit(98);
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 98)
		pc_unresolved("the child could not set a file size limit (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

#ifdef SIGXFSZ
static int xfsz_pending(void)
{
	sigset_t set;

	return sigpending(&set) == 0 && sigismember(&set, SIGXFSZ) == 1;
}
#endif

/* A socket bound to path (setup). */
static void make_socket(const char *path)
{
	struct sockaddr_un sa;
	int s = socket(AF_UNIX, SOCK_STREAM, 0);

	pc_check(s >= 0, "socket failed: %s", pc_errname(errno));
	memset(&sa, 0, sizeof sa);
	sa.sun_family = AF_UNIX;
	snprintf(sa.sun_path, sizeof sa.sun_path, "%s", path);
	pc_check(bind(s, (struct sockaddr *)&sa, sizeof sa) == 0, "bind(%s) failed: %s", path, pc_errname(errno));
}

#ifdef S_ISVTX
#define EXTRA (S_ISUID | S_ISGID | S_ISVTX)
#else
#define EXTRA (S_ISUID | S_ISGID)
#endif

int main(void)
{
	pc_begin("XSH.umask.004");
	{
		mode_t old, r;
		unsigned f1, d1;
		int fd;

		(void)umask(0751 | EXTRA);
		fd = open("f1", O_WRONLY | O_CREAT | O_EXCL, 0777);
		pc_check(fd >= 0 && mkdir("d1", 0777) == 0, "could not create the files: %s", pc_errname(errno));
		f1 = (unsigned)(lst("f1").st_mode & 07777);
		d1 = (unsigned)(lst("d1").st_mode & 07777);
		pc_test();
		(void)umask(0751 | EXTRA);
		old = umask(0);
		(void)umask(old);
		fd = open("f2", O_WRONLY | O_CREAT | O_EXCL, 0777);
		if (fd < 0 || mkdir("d2", 0777) != 0)
			pc_unresolved("could not create the files: %s", pc_errname(errno));
		r = umask(0);
		pc_check((r & 0777) == 0751, "after the round trip the mask is %#o, expected 0751", (unsigned)r);
		pc_check((lst("f2").st_mode & 07777) == f1, "the file has bits %#o after the round trip, %#o before",
			 (unsigned)(lst("f2").st_mode & 07777), f1);
		pc_check((lst("d2").st_mode & 07777) == d1, "the directory has bits %#o after the round trip, %#o before",
			 (unsigned)(lst("d2").st_mode & 07777), d1);
	}
	pc_pass();
}
