/*
 * With every descriptor in use, creat() fails with EMFILE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.018
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
	struct rlimit rl, old;
	struct stat st;
	int i, src, lim, top, fd, e;

	pc_begin("XSH.creat.018");
	pc_make_file("f", "x");
	src = open("f", O_RDONLY);
	pc_check(src >= 0, "cannot open f");
	lim = pc_lowest_free_fd() + 2;
	if (lim < 20)
		lim = 20;	/* not below {_POSIX_OPEN_MAX} */
	pc_check(getrlimit(RLIMIT_NOFILE, &old) == 0, "getrlimit failed");
	rl = old;
	top = rl.rlim_cur == RLIM_INFINITY || rl.rlim_cur > 65536 ? 65536 : (int)rl.rlim_cur;
	for (i = 0; i < top; i++)
		if (fcntl(i, F_GETFD) != -1 && i + 1 > lim)
			lim = i + 1;
	if (lim > 4096)
		pc_unresolved("a descriptor near %d is open; too many to fill", lim);
	for (i = 0; i < lim; i++)
		if (fcntl(i, F_GETFD) == -1)
			pc_check(dup2(src, i) == i, "dup2 to %d failed", i);
	rl.rlim_cur = (rlim_t)lim;
	pc_check(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed");
	if (getrlimit(RLIMIT_NOFILE, &rl) != 0 || rl.rlim_cur != (rlim_t)lim)
		pc_unresolved("RLIMIT_NOFILE did not take the value %d", lim);
	pc_test();
	fd = creat("new", 0600);
	e = errno;
	(void)setrlimit(RLIMIT_NOFILE, &old);
	pc_check(fd == -1, "creat() succeeded with every descriptor in use");
	pc_check(e == EMFILE, "creat() failed with %s, not EMFILE", pc_errname(e));
	pc_check(lstat("new", &st) == -1, "the failed creat() created a file");
	pc_pass();
}
