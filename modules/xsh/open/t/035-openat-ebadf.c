/*
 * openat() with a closed descriptor fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.035
 * MUTANT    open.at_ignores_fd
 * VARIANT   open.accmode_bits
 * VARIANT   open.hidden_fds
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int bad, fd;

	pc_begin("XSH.open.035");
	pc_make_file("f", "C");
	bad = pc_lowest_free_fd();
	pc_test();
	fd = openat(bad, "f", O_RDONLY);
	pc_expect_err(fd, EBADF, "openat(closed descriptor, \"f\", O_RDONLY)");
	pc_pass();
}
