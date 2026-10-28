/*
 * In a writable directory with S_ISVTX set, the owner of the directory may
 * unlink, rename and rmdir entries that belong to another user and that it
 * cannot write.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.5-directory-protection.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define DIR_OWNER 65533
#define FILE_OWNER 65532

static int exists(const char *p)
{
	struct stat sb;

	return lstat(p, &sb) == 0;
}

static void give(const char *p, uid_t u, gid_t g)
{
	if (chown(p, u, g) != 0)
		pc_unresolved("chown(%s) failed: %s", p, pc_errname(errno));
}

/* The child: the directory's owner, with no privilege and no extra groups. */
static int as_dir_owner(void)
{
	if (unlink("sd/f1") != 0)
		return 10 + (errno < 39 ? errno : 39);
	if (rename("sd/f2", "sd/f2b") != 0)
		return 50 + (errno < 39 ? errno : 39);
	return 0;
}

static int as_dir_owner_rmdir(void)
{
	return rmdir("sd/d3") == 0 ? 0 : 1;
}

static int run(int (*fn)(void))
{
	pid_t kid = fork();
	int st;

	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		(void)pc_drop_groups();
		if (setgid(DIR_OWNER) != 0 || setuid(DIR_OWNER) != 0)
			_exit(99);
		if (geteuid() == 0 || access("sd", W_OK) != 0)
			_exit(98);
		/* the entries must be ones this user cannot write */
		if (access("sd/f1", W_OK) == 0 || access("sd/f2", W_OK) == 0)
			_exit(97);
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) >= 97)
		pc_unresolved("the unprivileged child could not run as arranged (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

int main(void)
{
	int r;

	pc_begin("XBD.4.5-directory-protection.001");
	if (geteuid() != 0)
		pc_unresolved("needs root to make files that belong to other users");
	if (chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	pc_make_dir("sd");
	pc_make_file("sd/f1", "1");
	pc_make_file("sd/f2", "2");
	pc_make_dir("sd/d3");
	give("sd/f1", FILE_OWNER, FILE_OWNER);
	give("sd/f2", FILE_OWNER, FILE_OWNER);
	give("sd/d3", FILE_OWNER, FILE_OWNER);
	if (chmod("sd/f1", 0644) != 0 || chmod("sd/f2", 0644) != 0 || chmod("sd/d3", 0755) != 0 ||
	    chmod("sd", 01755) != 0)
		pc_unresolved("chmod failed: %s", pc_errname(errno));
	give("sd", DIR_OWNER, DIR_OWNER);
	pc_test();
	r = run(as_dir_owner);
	if (r >= 50)
		pc_fail("rename() of another user's file in a sticky directory by the directory's owner failed with %s", pc_errname(r - 50));
	if (r >= 10)
		pc_fail("unlink() of another user's file in a sticky directory by the directory's owner failed with %s", pc_errname(r - 10));
	pc_check(!exists("sd/f1"), "sd/f1 still exists after the directory's owner unlinked it");
	pc_check(!exists("sd/f2") && exists("sd/f2b"), "sd/f2 was not renamed to sd/f2b");
	r = run(as_dir_owner_rmdir);
	pc_check(r == 0, "rmdir() of another user's empty directory in a sticky directory by the directory's owner failed");
	pc_check(!exists("sd/d3"), "sd/d3 still exists after rmdir()");
	pc_pass();
}
