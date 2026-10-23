/*
 * A small non-blocking pipe write with room is complete.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.013
 * MUTANT    write.nonblock_short
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/* {PIPE_BUF}: from <limits.h>, else from fpathconf() on the pipe. */
static long pipe_buf(int fd)
{
#ifdef PIPE_BUF
	(void)fd;
	return PIPE_BUF;
#else
	long v = fpathconf(fd, _PC_PIPE_BUF);
	if (v <= 0)
		pc_unresolved("{PIPE_BUF} is indeterminate");
	return v;
#endif
}

int main(void)
{
	long pb;
	int p[2], fl;
	char *buf;

	pc_begin("XSH.write.013");
	pc_check(pipe(p) == 0, "pipe failed");
	pb = pipe_buf(p[1]);
	fl = fcntl(p[1], F_GETFL);
	pc_check(fl != -1 && fcntl(p[1], F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	buf = calloc(1, pb);
	pc_check(buf != NULL, "out of memory");
	pc_test();
	pc_check(write(p[1], buf, pb) == pb, "non-blocking write of {PIPE_BUF} bytes to an empty pipe was short");
	{
		long got = 0;
		ssize_t r;
		while (got < pb && (r = read(p[0], buf, (size_t)(pb - got))) > 0)
			got += r;
		pc_check(got == pb, "only %ld of the bytes reached the reader", got);
	}
	pc_pass();
}
