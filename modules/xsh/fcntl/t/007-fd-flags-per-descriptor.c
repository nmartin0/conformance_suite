/*
 * File descriptor flags belong to one descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.007
 * MUTANT    fcntl.setfd_affects_all
 * MUTANT    fcntl.setfd_ignored
 * VARIANT   fcntl.ok_other
 * VARIANT   fcntl.cloexec_bit
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a, b, f;

	pc_begin("XSH.fcntl.007");
	pc_make_file("f", "x");
	a = open("f", O_RDONLY);
	pc_check(a >= 0, "cannot open f");
	b = dup(a);
	pc_check(b >= 0, "dup failed");
	pc_test();
	pc_expect_ok(fcntl(a, F_SETFD, FD_CLOEXEC), "fcntl(F_SETFD, FD_CLOEXEC)");
	f = fcntl(a, F_GETFD);
	pc_check(f >= 0, "F_GETFD returned %d", f);
	pc_check(f & FD_CLOEXEC, "FD_CLOEXEC not set");
	f = fcntl(b, F_GETFD);
	pc_check(f >= 0 && !(f & FD_CLOEXEC), "FD_CLOEXEC also set on the duplicate");
	pc_expect_ok(fcntl(a, F_SETFD, 0), "fcntl(F_SETFD, 0)");
	f = fcntl(a, F_GETFD);
	pc_check(f >= 0 && !(f & FD_CLOEXEC), "FD_CLOEXEC not cleared");
	pc_pass();
}
