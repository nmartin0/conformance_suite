/*
 * dup3(fd, fd) gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.012
 * MUTANT    dup.dup3_same_ok
 * VARIANT   dup.hidden_fds
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.dup.012");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_expect_err(dup3(fd, fd, 0), EINVAL, "dup3(fd, fd, 0)");
	pc_check(fcntl(fd, F_GETFD) != -1, "fd was closed");
	pc_pass();
}
