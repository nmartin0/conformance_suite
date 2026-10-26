/*
 * {PIPE_BUF} of a pipe and of a FIFO is -1 (no limit) or at least 512.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpathconf.003
 * MUTANT    fpathconf.pipe_buf_256
 * MUTANT    fpathconf.pipe_einval
 * VARIANT   fpathconf.unassociated
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define SENTINEL 12345

static void check(long r, int e, const char *what)
{
	if (r == -1) {
		pc_check(e == SENTINEL, "%s of _PC_PIPE_BUF failed with %s", what, pc_errname(e));
		return;
	}
	pc_check(r >= 512, "%s of _PC_PIPE_BUF returned %ld, below {_POSIX_PIPE_BUF} (512)", what, r);
}

int main(void)
{
	int p[2], f;
	long r;

	pc_begin("XSH.fpathconf.003");
	pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
	pc_check(mkfifo("fifo", 0600) == 0, "mkfifo failed: %s", pc_errname(errno));
	f = open("fifo", O_RDONLY | O_NONBLOCK);
	pc_check(f >= 0, "cannot open fifo: %s", pc_errname(errno));
	pc_test();
	errno = SENTINEL;
	r = fpathconf(p[0], _PC_PIPE_BUF);
	check(r, errno, "fpathconf() on a pipe's read end");
	errno = SENTINEL;
	r = fpathconf(p[1], _PC_PIPE_BUF);
	check(r, errno, "fpathconf() on a pipe's write end");
	errno = SENTINEL;
	r = fpathconf(f, _PC_PIPE_BUF);
	check(r, errno, "fpathconf() on a FIFO");
	errno = SENTINEL;
	r = pathconf("fifo", _PC_PIPE_BUF);
	check(r, errno, "pathconf() on a FIFO");
	pc_pass();
}
