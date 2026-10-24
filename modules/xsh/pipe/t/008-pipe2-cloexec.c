/*
 * pipe2() with O_CLOEXEC sets FD_CLOEXEC on both ends.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pipe.008
 * MUTANT    pipe.p2_flags_ignored
 * MUTANT    pipe.p2_cloexec_one_end
 * VARIANT   pipe.high_read_end
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int p[2], i, f;

	pc_begin("XSH.pipe.008");
	pc_test();
	pc_expect_ok(pipe2(p, O_CLOEXEC), "pipe2(p, O_CLOEXEC)");
	for (i = 0; i < 2; i++) {
		f = fcntl(p[i], F_GETFD);
		pc_check(f != -1 && (f & FD_CLOEXEC), "FD_CLOEXEC clear on fildes[%d]", i);
	}
	pc_pass();
}
