/*
 * stat() reports each file type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.006
 * MUTANT    stat.type_reg
 * MUTANT    stat.fifo_as_chr
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

	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
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
	pc_begin("XSH.fstatat.006");
	{
		struct sockaddr_un sa;
		struct stat st;
		const char *name[3] = { "d", "fifo", "sock" };
		int s, i, n;

		pc_make_dir("d");
		pc_check(mkfifo("fifo", 0600) == 0, "mkfifo failed: %s", pc_errname(errno));
		s = socket(AF_UNIX, SOCK_STREAM, 0);
		pc_check(s >= 0, "socket failed: %s", pc_errname(errno));
		memset(&sa, 0, sizeof sa);
		sa.sun_family = AF_UNIX;
		strcpy(sa.sun_path, "sock");
		pc_check(bind(s, (struct sockaddr *)&sa, sizeof sa) == 0, "bind failed: %s", pc_errname(errno));
		pc_test();
		for (i = 0; i < 3; i++) {
			pc_check(stat(name[i], &st) == 0, "stat(%s) failed: %s", name[i], pc_errname(errno));
			n = !!S_ISDIR(st.st_mode) + !!S_ISFIFO(st.st_mode) + !!S_ISCHR(st.st_mode) +
			    !!S_ISSOCK(st.st_mode) + !!S_ISREG(st.st_mode) + !!S_ISBLK(st.st_mode) +
			    !!S_ISLNK(st.st_mode);
			pc_check(n == 1, "%s: %d type macros are true for st_mode %#o", name[i], n, (unsigned)st.st_mode);
			pc_check(i != 0 || S_ISDIR(st.st_mode), "a directory is not S_ISDIR (st_mode %#o)", (unsigned)st.st_mode);
			pc_check(i != 1 || S_ISFIFO(st.st_mode), "a FIFO is not S_ISFIFO (st_mode %#o)", (unsigned)st.st_mode);
			pc_check(i != 2 || S_ISSOCK(st.st_mode), "a socket is not S_ISSOCK (st_mode %#o)", (unsigned)st.st_mode);
		}
	}
	pc_pass();
}
