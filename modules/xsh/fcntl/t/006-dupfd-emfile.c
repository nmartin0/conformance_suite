/*
 * F_DUPFD with no free descriptor at or above arg gives EMFILE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.006
 * MUTANT    fcntl.emfile_as_einval
 * MUTANT    fcntl.dupfd_ignores_arg
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>

int main(void)
{
	struct rlimit rl;
	int fd, lim, i;

	pc_begin("XSH.fcntl.006");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	lim = pc_lowest_free_fd() + 4;
	for (i = 0; i < 1024; i++)
		if (fcntl(i, F_GETFD) != -1 && i + 1 > lim)
			lim = i + 1;
	if (lim > 256)
		pc_unresolved("a descriptor near %d is open", lim);
	pc_check(dup2(fd, lim - 1) == lim - 1, "cannot fill slot %d", lim - 1);
	pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	rl.rlim_cur = (rlim_t)lim;
	pc_check(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed");
	pc_check(pc_lowest_free_fd() < lim - 1, "no lower slot is free");
	pc_test();
	pc_expect_err(fcntl(fd, F_DUPFD, lim - 1), EMFILE,
	    "fcntl(F_DUPFD, last slot) with that slot in use");
	pc_pass();
}
