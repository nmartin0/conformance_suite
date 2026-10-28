/*
 * In a writable directory with S_ISVTX set that belongs to one user, a
 * process with appropriate privileges may unlink, rename and rmdir entries
 * that belong to another user.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.5-directory-protection.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

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

int main(void)
{
	pc_begin("XBD.4.5-directory-protection.002");
	if (geteuid() != 0)
		pc_unresolved("needs root");
	pc_make_dir("sd");
	pc_make_file("sd/f1", "1");
	pc_make_file("sd/f2", "2");
	pc_make_dir("sd/d3");
	give("sd/f1", FILE_OWNER, FILE_OWNER);
	give("sd/f2", FILE_OWNER, FILE_OWNER);
	give("sd/d3", FILE_OWNER, FILE_OWNER);
	if (chmod("sd/f1", 0400) != 0 || chmod("sd/f2", 0400) != 0 || chmod("sd/d3", 0500) != 0 ||
	    chmod("sd", 01777) != 0)
		pc_unresolved("chmod failed: %s", pc_errname(errno));
	give("sd", DIR_OWNER, DIR_OWNER);
	pc_test();
	pc_check(unlink("sd/f1") == 0, "unlink() by a privileged process of another user's file in a sticky directory failed: %s", pc_errname(errno));
	pc_check(rename("sd/f2", "sd/f2b") == 0, "rename() by a privileged process of another user's file in a sticky directory failed: %s", pc_errname(errno));
	pc_check(rmdir("sd/d3") == 0, "rmdir() by a privileged process of another user's directory in a sticky directory failed: %s", pc_errname(errno));
	pc_check(!exists("sd/f1") && !exists("sd/f2") && exists("sd/f2b") && !exists("sd/d3"), "the entries are not as expected after the calls");
	pc_pass();
}
