/*
 * dup2() to an out-of-range descriptor gives EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.010
 * MUTANT    dup.dup2_range_einval
 * MUTANT    dup.dup2_max_ok
 * VARIANT   dup.hidden_fds
 * VARIANT   dup.open_max_indeterminate
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	long max;
	int fd;

	pc_begin("XSH.dup.010");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	max = sysconf(_SC_OPEN_MAX);
	pc_test();
	pc_expect_err(dup2(fd, -1), EBADF, "dup2(fd, -1)");
	if (max > 0 && max < 0x7fffffff)
		pc_expect_err(dup2(fd, (int)max), EBADF, "dup2(fd, {OPEN_MAX})");
	else
		pc_note("{OPEN_MAX} case skipped: sysconf(_SC_OPEN_MAX) gave %ld", max);
	pc_pass();
}
