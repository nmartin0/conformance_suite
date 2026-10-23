/*
 * dup() clears FD_CLOEXEC on the new descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.003
 * MUTANT    dup.keeps_cloexec
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, d, f;

	pc_begin("XSH.dup.003");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(fcntl(fd, F_SETFD, FD_CLOEXEC) == 0, "cannot set FD_CLOEXEC");
	pc_test();
	d = dup(fd);
	pc_expect_ok(d, "dup()");
	f = fcntl(d, F_GETFD);
	pc_expect_ok(f, "fcntl(F_GETFD)");
	pc_check(!(f & FD_CLOEXEC), "FD_CLOEXEC is set on the duplicate");
	pc_pass();
}
