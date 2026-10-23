/*
 * stat() describes a regular file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.001
 * MUTANT    stat.size_off_by_one
 * MUTANT    stat.type_dir
 * MUTANT    stat.mode_bits_wrong
 * MUTANT    stat.nlink_zero
 * MUTANT    stat.ino_wrong
 * MUTANT    stat.uid_wrong
 * MUTANT    stat.returns_one
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
}

/*
 * Run fn in a child that has no appropriate privileges: when the test runs
 * as root, the child takes user and group 65534 (the working directory is
 * made searchable for it; relative paths need nothing above it). Returns
 * what fn returns (0 to 98); a child that cannot drop privileges, or that
 * dies, makes the test UNRESOLVED.
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
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
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

int main(void)
{
	pc_begin("XSH.fstatat.001");
	{
		struct stat st, ref, dir;
		int fd, r;

		fd = open("f", O_WRONLY | O_CREAT | O_EXCL, 0640);
		pc_check(fd >= 0, "open failed: %s", pc_errname(errno));
		pc_check(write(fd, "12345", 5) == 5, "write failed");
		pc_check(close(fd) == 0, "close failed");
		ref = via_fd("f", O_RDONLY);
		dir = via_fd(".", O_RDONLY);
		memset(&st, 0, sizeof st);
		pc_test();
		r = stat("f", &st);
		pc_check(r == 0, "stat() returned %d (%s)", r, pc_errname(errno));
		pc_check(S_ISREG(st.st_mode), "st_mode %#o is not a regular file", (unsigned)st.st_mode);
		pc_check((st.st_mode & 07777) == 0640, "permission bits %#o, created 0640", (unsigned)(st.st_mode & 07777));
		pc_check(st.st_size == 5, "st_size %lld, 5 bytes written", (long long)st.st_size);
		pc_check(st.st_nlink == 1, "st_nlink %lu for a file with one link", (unsigned long)st.st_nlink);
		pc_check(st.st_uid == geteuid(), "st_uid %lu, effective user %lu", (unsigned long)st.st_uid,
			 (unsigned long)geteuid());
		pc_check(st.st_gid == getegid() || st.st_gid == dir.st_gid,
			 "st_gid %lu is neither the effective group nor the directory's", (unsigned long)st.st_gid);
		pc_check(same_file(&st, &ref), "st_dev/st_ino differ from the open file's");
		pc_check(st.st_mode == ref.st_mode && st.st_size == ref.st_size, "stat() and fstat() disagree");
	}
	pc_pass();
}
