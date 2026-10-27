/*
 * creat() without write or search permission on the directory fails with EACCES.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.016
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
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/*
 * Run fn in a child that has no appropriate privileges. When the suite runs
 * as root the child takes user and group 65534 (the working directory is
 * made searchable for it first, by pc_searchable() in the setup phase).
 * Returns what fn returns (0 to 96). A child that cannot drop privileges,
 * that still has them (it can read a mode 0 file) or that dies makes the
 * test UNRESOLVED.
 */
static void pc_searchable(void)
{
	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	pc_make_file(".pc-priv", "");
	if (chmod(".pc-priv", 0) != 0)
		pc_unresolved("chmod(.pc-priv) failed: %s", pc_errname(errno));
}

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
		pc_unresolved("the child still has appropriate privileges");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 98)
		pc_unresolved("the unprivileged child kept supplementary groups");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

static int one(const char *p)
{
	int fd = creat(p, 0600);

	if (fd >= 0)
		return 1;
	return errno == EACCES ? 0 : 2;
}

static int child(void)
{
	int v = one("w/f");

	return v != 0 ? v : one("s/f");
}

int main(void)
{
	struct stat st;

	pc_begin("XSH.creat.016");
	pc_searchable();
	pc_make_dir("w");
	pc_make_dir("s");
	pc_check(chmod("w", 0555) == 0 && chmod("s", 0600) == 0, "chmod failed");
	pc_test();
	switch (unprivileged(child)) {
	case 0:
		break;
	case 1:
		pc_fail("creat() succeeded without permission");
	default:
		pc_fail("creat() without permission failed with the wrong error");
	}
	if (chmod("s", 0700) != 0)
		pc_unresolved("chmod failed: %s", pc_errname(errno));
	pc_check(lstat("w/f", &st) == -1 && lstat("s/f", &st) == -1, "a failed creat() created a file");
	pc_pass();
}
