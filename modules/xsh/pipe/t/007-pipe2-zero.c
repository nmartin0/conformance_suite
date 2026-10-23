/*
 * pipe2(fildes, 0) is pipe().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pipe.007
 * MUTANT    pipe.p2_cloexec_always
 * MUTANT    pipe.swapped
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int p[2], i, f;
	char c;

	pc_begin("XSH.pipe.007");
	pc_test();
	pc_expect_ok(pipe2(p, 0), "pipe2(p, 0)");
	pc_check(write(p[1], "z", 1) == 1 && read(p[0], &c, 1) == 1 && c == 'z',
	    "data did not pass from fildes[1] to fildes[0]");
	for (i = 0; i < 2; i++) {
		f = fcntl(p[i], F_GETFD);
		pc_check(f != -1 && !(f & FD_CLOEXEC), "FD_CLOEXEC set on fildes[%d]", i);
		f = fcntl(p[i], F_GETFL);
		pc_check(f != -1 && !(f & O_NONBLOCK), "O_NONBLOCK set on fildes[%d]", i);
	}
	pc_pass();
}
