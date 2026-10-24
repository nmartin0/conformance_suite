/*
 * openat() with a non-directory descriptor fails with ENOTDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.036
 * MUTANT    open.at_ignores_fd
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int ffd, fd;

	pc_begin("XSH.open.036");
	pc_make_file("f", "C");
	ffd = open("f", O_RDONLY);
	pc_check(ffd >= 0, "cannot open f");
	pc_test();
	fd = openat(ffd, "f", O_RDONLY);
	pc_expect_err(fd, ENOTDIR, "openat(regular file descriptor, \"f\", O_RDONLY)");
	pc_pass();
}
