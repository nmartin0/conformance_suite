/*
 * F_DUPFD with an out-of-range arg gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.005
 * MUTANT    fcntl.dupfd_neg_ok
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	long max;
	int fd;

	pc_begin("XSH.fcntl.005");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	max = sysconf(_SC_OPEN_MAX);
	pc_test();
	pc_expect_err(fcntl(fd, F_DUPFD, -1), EINVAL, "fcntl(F_DUPFD, -1)");
	if (max > 0 && max < 0x7fffffff)
		pc_expect_err(fcntl(fd, F_DUPFD, (int)max), EINVAL, "fcntl(F_DUPFD, {OPEN_MAX})");
	else
		pc_note("{OPEN_MAX} case skipped: sysconf(_SC_OPEN_MAX) gave %ld", max);
	pc_pass();
}
