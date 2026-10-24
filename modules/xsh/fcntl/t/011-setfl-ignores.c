/*
 * F_SETFL ignores access-mode and creation bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.011
 * MUTANT    fcntl.setfl_creation_flags_act
 * VARIANT   fcntl.ok_other
 * VARIANT   fcntl.cloexec_bit
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, fl, f;

	pc_begin("XSH.fcntl.011");
	pc_make_file("f", "keep");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	fl = fcntl(fd, F_GETFL);
	pc_check(fl != -1, "F_GETFL failed");
	pc_test();
	pc_expect_ok(fcntl(fd, F_SETFL, (fl & ~O_ACCMODE) | O_RDWR | O_TRUNC | O_CLOEXEC),
	    "fcntl(F_SETFL, O_RDWR|O_TRUNC|O_CLOEXEC)");
	fl = fcntl(fd, F_GETFL);
	pc_check(fl != -1 && (fl & O_ACCMODE) == O_RDONLY, "the access mode changed");
	pc_expect_err(write(fd, "x", 1), EBADF, "write() on the read-only descriptor");
	pc_check(pc_file_equals("f", "keep"), "the file was truncated or written");
	f = fcntl(fd, F_GETFD);
	pc_check(f != -1 && !(f & FD_CLOEXEC), "F_SETFL with O_CLOEXEC set FD_CLOEXEC");
	pc_pass();
}
