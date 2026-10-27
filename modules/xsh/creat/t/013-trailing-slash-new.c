/*
 * creat("new/") with nothing named new fails with ENOENT or ENOTDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.013
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
	struct stat st;
	int fd, e;

	pc_begin("XSH.creat.013");
	pc_test();
	fd = creat("new/", 0600);
	e = errno;
	pc_check(fd == -1, "creat(\"new/\") succeeded");
	pc_check(e == ENOENT || e == ENOTDIR, "creat(\"new/\") failed with %s, not ENOENT or ENOTDIR", pc_errname(e));
	pc_check(lstat("new", &st) == -1, "creat(\"new/\") created something");
	pc_pass();
}
