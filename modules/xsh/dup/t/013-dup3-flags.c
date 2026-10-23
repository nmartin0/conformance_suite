/*
 * dup3() takes FD_CLOEXEC from its flag argument only.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.013
 * MUTANT    dup.dup3_cloexec_ignored
 * MUTANT    dup.dup3_keeps_cloexec
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a, b, c, f;

	pc_begin("XSH.dup.013");
	pc_make_file("f", "x");
	a = open("f", O_RDONLY | O_CLOEXEC);
	pc_check(a >= 0, "cannot open f");
	b = pc_lowest_free_fd() + 2;
	c = b + 1;
	pc_check(fcntl(b, F_GETFD) == -1 && fcntl(c, F_GETFD) == -1,
	    "targets are not free");
	pc_test();
	pc_check(dup3(a, b, 0) == b, "dup3(a, b, 0) failed");
	f = fcntl(b, F_GETFD);
	pc_check(f != -1 && !(f & FD_CLOEXEC), "dup3(..., 0): FD_CLOEXEC set");
	pc_check(fcntl(a, F_SETFD, 0) == 0, "cannot clear FD_CLOEXEC on fildes");
	pc_check(dup3(a, c, O_CLOEXEC) == c, "dup3(a, c, O_CLOEXEC) failed");
	f = fcntl(c, F_GETFD);
	pc_check(f != -1 && (f & FD_CLOEXEC), "dup3(..., O_CLOEXEC): FD_CLOEXEC clear");
	pc_pass();
}
