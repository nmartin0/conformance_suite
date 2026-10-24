/*
 * pipe() allocates the lowest free descriptors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pipe.002
 * MUTANT    pipe.not_lowest
 * VARIANT   pipe.high_read_end
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int p[2], fd, lo, mid;

	pc_begin("XSH.pipe.002");
	pc_make_file("f", "x");
	lo = pc_lowest_free_fd();
	fd = open("f", O_RDONLY);
	pc_check(fd == lo, "cannot fill slot %d", lo);
	mid = fcntl(fd, F_DUPFD, lo + 1);
	pc_check(mid == lo + 1, "cannot fill slot %d", lo + 1);
	pc_check(close(fd) == 0, "close failed");
	pc_check(fcntl(lo + 2, F_GETFD) == -1, "slot %d is not free", lo + 2);
	pc_test();
	pc_expect_ok(pipe(p), "pipe()");
	pc_check((p[0] == lo && p[1] == lo + 2) || (p[0] == lo + 2 && p[1] == lo),
	    "got %d and %d, expected %d and %d", p[0], p[1], lo, lo + 2);
	pc_pass();
}
