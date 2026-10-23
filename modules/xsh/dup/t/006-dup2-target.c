/*
 * dup2() to a free descriptor returns it, sharing the description.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.006
 * MUTANT    dup.dup2_picks_lowest
 * MUTANT    dup.dup2_fresh_description
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, t, r;
	char c;

	pc_begin("XSH.dup.006");
	pc_make_file("f", "abc");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	t = pc_lowest_free_fd() + 3;
	pc_check(fcntl(t, F_GETFD) == -1, "target %d is not free", t);
	pc_test();
	r = dup2(fd, t);
	pc_check(r == t, "dup2(fd, %d) returned %d", t, r);
	pc_check(read(fd, &c, 1) == 1 && c == 'a', "read on the original failed");
	pc_check(read(t, &c, 1) == 1, "read on fildes2 failed");
	pc_check(c == 'b', "fildes2 read '%c': the offset is not shared", c);
	pc_pass();
}
