/*
 * dup() with every descriptor in use gives EMFILE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.005
 * MUTANT    dup.emfile_as_enfile
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>

int main(void)
{
	struct rlimit rl;
	int i, src, lim;

	pc_begin("XSH.dup.005");
	pc_make_file("f", "x");
	src = open("f", O_RDONLY);
	pc_check(src >= 0, "cannot open f");
	lim = pc_lowest_free_fd() + 2;
	if (lim < 8)
		lim = 8;
	for (i = 0; i < lim; i++)
		if (fcntl(i, F_GETFD) == -1)
			pc_check(dup2(src, i) == i, "dup2 to %d failed", i);
	pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	rl.rlim_cur = (rlim_t)lim;
	pc_check(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed");
	for (i = 0; i < lim; i++)
		pc_check(fcntl(i, F_GETFD) != -1, "descriptor %d is not in use", i);
	pc_test();
	pc_expect_err(dup(src), EMFILE, "dup() with every descriptor in use");
	pc_pass();
}
