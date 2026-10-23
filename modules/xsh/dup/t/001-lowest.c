/*
 * dup() returns the lowest free descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.001
 * MUTANT    dup.not_lowest
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, gap, hi, d;

	pc_begin("XSH.dup.001");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	gap = pc_lowest_free_fd();
	hi = fcntl(fd, F_DUPFD, gap + 1);
	pc_check(hi > gap, "cannot place a descriptor above the gap");
	pc_check(fcntl(gap, F_GETFD) == -1, "the gap is not free");
	pc_test();
	d = dup(hi);
	pc_expect_ok(d, "dup()");
	pc_check(d == gap, "dup() returned %d, the lowest free descriptor is %d", d, gap);
	pc_pass();
}
