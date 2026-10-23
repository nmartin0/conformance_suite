/*
 * pipe() with one free descriptor gives EMFILE and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pipe.006
 * MUTANT    pipe.emfile_as_enfile
 * MUTANT    pipe.emfile_scribbles
 * MUTANT    pipe.emfile_leaks
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>

int main(void)
{
	struct rlimit rl;
	int p[2] = { -7, -7 }, i, src, lim;

	pc_begin("XSH.pipe.006");
	pc_make_file("f", "x");
	src = open("f", O_RDONLY);
	pc_check(src >= 0, "cannot open f");
	lim = pc_lowest_free_fd() + 3;
	if (lim < 8)
		lim = 8;
	for (i = 0; i < 1024; i++)
		if (fcntl(i, F_GETFD) != -1 && i + 2 > lim)
			lim = i + 2;
	if (lim > 256)
		pc_unresolved("a descriptor near %d is open; too many to fill", lim);
	/* Fill every slot below lim except the last. */
	for (i = 0; i < lim - 1; i++)
		if (fcntl(i, F_GETFD) == -1)
			pc_check(dup2(src, i) == i, "dup2 to %d failed", i);
	pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	rl.rlim_cur = (rlim_t)lim;
	pc_check(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed");
	pc_check(fcntl(lim - 1, F_GETFD) == -1, "the last slot is not free");
	pc_test();
	pc_expect_err(pipe(p), EMFILE, "pipe() with one descriptor free");
	pc_check(p[0] == -7 && p[1] == -7, "fildes changed to %d, %d", p[0], p[1]);
	pc_check(fcntl(lim - 1, F_GETFD) == -1, "a descriptor was allocated");
	pc_pass();
}
