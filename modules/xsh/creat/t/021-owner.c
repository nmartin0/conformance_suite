/*
 * A file made by creat() belongs to the effective user and a permitted group.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.021
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

int main(void)
{
	struct stat st, dir;

	pc_begin("XSH.creat.021");
	pc_require_space();
	pc_check(stat(".", &dir) == 0, "stat of the directory failed");
	pc_test();
	pc_expect_ok(creat("new", 0600), "creat(\"new\")");
	pc_check(stat("new", &st) == 0, "stat failed");
	pc_check(st.st_uid == geteuid(), "the owner is %ld, not the effective user ID", (long)st.st_uid);
	pc_check(st.st_gid == getegid() || st.st_gid == dir.st_gid,
	    "the group is neither the effective group ID nor the directory's");
	pc_pass();
}
