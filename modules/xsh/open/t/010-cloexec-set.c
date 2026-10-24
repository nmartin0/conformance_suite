/*
 * With O_CLOEXEC, FD_CLOEXEC is set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.010
 * MUTANT    open.cloexec_ignored
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	int fd, fl;

	pc_begin("XSH.open.010");
	pc_make_file("f", "x");
	pc_test();
	fd = open("f", O_RDONLY | O_CLOEXEC);
	pc_expect_ok(fd, "open(\"f\", O_RDONLY|O_CLOEXEC)");
	fl = fcntl(fd, F_GETFD);
	pc_expect_ok(fl, "fcntl(F_GETFD)");
	pc_check((fl & FD_CLOEXEC) != 0, "FD_CLOEXEC is clear");
	pc_pass();
}
