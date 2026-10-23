/*
 * pipe() leaves FD_CLOEXEC and O_NONBLOCK clear.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pipe.003
 * MUTANT    pipe.cloexec_set
 * MUTANT    pipe.nonblock_set
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int p[2], i, f;

	pc_begin("XSH.pipe.003");
	pc_test();
	pc_expect_ok(pipe(p), "pipe()");
	for (i = 0; i < 2; i++) {
		f = fcntl(p[i], F_GETFD);
		pc_check(f != -1 && !(f & FD_CLOEXEC), "FD_CLOEXEC set on fildes[%d]", i);
		f = fcntl(p[i], F_GETFL);
		pc_check(f != -1 && !(f & O_NONBLOCK), "O_NONBLOCK set on fildes[%d]", i);
	}
	pc_pass();
}
