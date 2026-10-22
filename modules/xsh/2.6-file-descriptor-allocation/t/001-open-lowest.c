/*
 * open() returns the lowest-numbered descriptor not open in the process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.6-file-descriptor-allocation.001
 * MUTANT    open.not_lowest
 * MUTANT    open.after_highest
 *
 * Setup opens a descriptor well above the lowest free one (with
 * fcntl F_DUPFD, so close() is not involved), leaving a gap: an allocator
 * that hands out "one above the highest" then gives a different answer
 * from one that hands out the lowest free number.
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int low, high, src, fd;

	pc_begin("XSH.2.6-file-descriptor-allocation.001");
	pc_make_file("f", "x");
	low = pc_lowest_free_fd();
	src = open("f", O_RDONLY);
	pc_check(src == low, "setup open returned %d, not %d", src, low);
	high = fcntl(src, F_DUPFD, low + 5);
	pc_check(high >= low + 5, "fcntl(F_DUPFD, %d) returned %d", low + 5,
	    high);
	low = pc_lowest_free_fd();
	pc_check(low < high, "no gap below descriptor %d", high);
	pc_test();
	fd = open("f", O_RDONLY);
	pc_expect_ok(fd, "open(\"f\", O_RDONLY)");
	pc_check(fd == low, "open returned %d; lowest free was %d (highest "
	    "open was %d)", fd, low, high);
	pc_pass();
}
