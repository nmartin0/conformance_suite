/*
 * The descriptor from creat() has FD_CLOEXEC (and FD_CLOFORK) clear.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.007
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
	int fd, fl;

	pc_begin("XSH.creat.007");
	pc_require_space();
	pc_test();
	fd = creat("new", 0600);
	pc_expect_ok(fd, "creat(\"new\")");
	fl = fcntl(fd, F_GETFD);
	pc_check(fl != -1, "F_GETFD failed");
	pc_check(!(fl & FD_CLOEXEC), "FD_CLOEXEC is set");
#ifdef FD_CLOFORK
	pc_check(!(fl & FD_CLOFORK), "FD_CLOFORK is set");
#endif
	pc_pass();
}
