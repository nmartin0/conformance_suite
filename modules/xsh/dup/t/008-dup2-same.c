/*
 * dup2(fd, fd) returns fd and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.008
 * MUTANT    dup.dup2_same_closes
 * MUTANT    dup.dup2_same_clears_cloexec
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, r, f;

	pc_begin("XSH.dup.008");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(fcntl(fd, F_SETFD, FD_CLOEXEC) == 0, "cannot set FD_CLOEXEC");
	pc_test();
	r = dup2(fd, fd);
	pc_check(r == fd, "dup2(fd, fd) returned %d, expected %d", r, fd);
	f = fcntl(fd, F_GETFD);
	pc_check(f != -1, "fd was closed by dup2(fd, fd)");
	pc_check(f & FD_CLOEXEC, "dup2(fd, fd) cleared FD_CLOEXEC");
	pc_pass();
}
