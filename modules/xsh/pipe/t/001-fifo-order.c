/*
 * pipe() makes a first-in-first-out channel from fildes[1] to fildes[0].
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pipe.001
 * MUTANT    pipe.swapped
 * VARIANT   pipe.high_read_end
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int p[2] = { -1, -1 }, r;
	char buf[8];

	pc_begin("XSH.pipe.001");
	pc_test();
	r = pipe(p);
	pc_check(r == 0, "pipe() returned %d (%s)", r, r == -1 ? pc_errname(errno) : "-");
	pc_check(p[0] >= 0 && p[1] >= 0 && p[0] != p[1], "descriptors %d and %d", p[0], p[1]);
	pc_check(write(p[1], "abc", 3) == 3, "write to fildes[1] failed");
	pc_check(write(p[1], "de", 2) == 2, "second write to fildes[1] failed");
	pc_check(read(p[0], buf, 5) == 5, "read from fildes[0] failed");
	pc_check(buf[0] == 'a' && buf[1] == 'b' && buf[2] == 'c' &&
	    buf[3] == 'd' && buf[4] == 'e', "data not read in the order written");
	pc_pass();
}
