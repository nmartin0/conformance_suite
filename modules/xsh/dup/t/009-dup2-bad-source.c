/*
 * dup2() from a closed descriptor gives EBADF and keeps fildes2 open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.009
 * MUTANT    dup.dup2_bad_src_closes
 * MUTANT    dup.ebadf_as_einval
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int t, bad;

	pc_begin("XSH.dup.009");
	pc_make_file("f", "x");
	t = open("f", O_RDONLY);
	pc_check(t >= 0, "cannot open f");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(dup2(bad, t), EBADF, "dup2(<not open>, open fd)");
	pc_check(fcntl(t, F_GETFD) != -1, "fildes2 was closed");
	pc_pass();
}
