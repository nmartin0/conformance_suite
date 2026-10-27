/*
 * creat() of an existing file truncates it and keeps its mode and owner.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.003
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
	struct stat before, after;
	int fd;

	pc_begin("XSH.creat.003");
	pc_make_file("f", "twelve bytes");
	pc_check(chmod("f", 0640) == 0, "chmod failed");
	pc_check(stat("f", &before) == 0, "stat failed");
	pc_test();
	fd = creat("f", 0600);
	pc_expect_ok(fd, "creat(\"f\") of an existing file");
	pc_check(stat("f", &after) == 0, "stat failed after creat()");
	pc_check(after.st_size == 0, "the length is %ld, not 0", (long)after.st_size);
	pc_check((after.st_mode & 07777) == 0640, "the mode changed to %o", (unsigned)(after.st_mode & 07777));
	pc_check(after.st_uid == before.st_uid && after.st_gid == before.st_gid, "the owner changed");
	pc_pass();
}
