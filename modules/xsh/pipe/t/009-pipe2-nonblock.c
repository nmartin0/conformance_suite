/*
 * pipe2() with O_NONBLOCK makes both ends non-blocking.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pipe.009
 * MUTANT    pipe.p2_nonblock_ignored
 * VARIANT   pipe.high_read_end
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
	int p[2], i, f;
	char c;

	pc_begin("XSH.pipe.009");
	alarm(10);	/* a blocking read would otherwise hang the test */
	pc_test();
	pc_expect_ok(pipe2(p, O_NONBLOCK), "pipe2(p, O_NONBLOCK)");
	for (i = 0; i < 2; i++) {
		f = fcntl(p[i], F_GETFL);
		pc_check(f != -1 && (f & O_NONBLOCK), "O_NONBLOCK clear on fildes[%d]", i);
	}
	pc_expect_err(read(p[0], &c, 1), EAGAIN, "read() of the empty pipe");
	pc_pass();
}
