/*
 * tmpfile() fails with EMFILE when every descriptor is in use.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tmpfile.005
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
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	pc_begin("XSH.tmpfile.005");
	{
		struct rlimit rl;
		int i, src, lim;
		FILE *f;

		pc_make_file("f", "x");
		src = open("f", O_RDONLY);
		pc_check(src >= 0, "cannot open f");
		lim = pc_lowest_free_fd() + 2;
		if (lim < 8)
			lim = 8;
		for (i = 0; i < 1024; i++)
			if (fcntl(i, F_GETFD) != -1 && i + 1 > lim)
				lim = i + 1;
		if (lim > 256)
			pc_unresolved("a descriptor near %d is open; too many to fill", lim);
		for (i = 0; i < lim; i++)
			if (fcntl(i, F_GETFD) == -1)
				if (!(dup2(src, i) == i))
					pc_unresolved("dup2 to %d failed", i);
		pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
		rl.rlim_cur = (rlim_t)lim;
		if (!(setrlimit(RLIMIT_NOFILE, &rl) == 0))
			pc_unresolved("setrlimit failed");
		pc_test();
		errno = 0;
		f = tmpfile();
		if (f != NULL)
			pc_fail("tmpfile() succeeded with every descriptor in use");
		pc_check(errno == EMFILE, "tmpfile() set errno %s, expected EMFILE", pc_errname(errno));
	}
	pc_pass();
}
