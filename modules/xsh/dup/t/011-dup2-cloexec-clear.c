/*
 * dup2() clears FD_CLOEXEC on fildes2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.011
 * MUTANT    dup.dup2_keeps_cloexec
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a, b, f;

	pc_begin("XSH.dup.011");
	pc_make_file("f", "x");
	a = open("f", O_RDONLY | O_CLOEXEC);
	b = open("f", O_RDONLY | O_CLOEXEC);
	pc_check(a >= 0 && b >= 0, "cannot open f");
	pc_test();
	pc_check(dup2(a, b) == b, "dup2() failed");
	f = fcntl(b, F_GETFD);
	pc_expect_ok(f, "fcntl(F_GETFD)");
	pc_check(!(f & FD_CLOEXEC), "FD_CLOEXEC is set on fildes2");
	pc_pass();
}
