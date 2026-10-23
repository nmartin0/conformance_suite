/*
 * dup() shares the offset and status flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.002
 * MUTANT    dup.fresh_description
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, d, fl;
	char c;

	pc_begin("XSH.dup.002");
	pc_make_file("f", "abc");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	d = dup(fd);
	pc_expect_ok(d, "dup()");
	pc_check(read(fd, &c, 1) == 1 && c == 'a', "read on the original failed");
	pc_check(read(d, &c, 1) == 1, "read on the duplicate failed");
	pc_check(c == 'b', "duplicate read '%c': the offset is not shared", c);
	pc_check(fcntl(d, F_SETFL, O_NONBLOCK) == 0, "F_SETFL on the duplicate failed");
	fl = fcntl(fd, F_GETFL);
	pc_check(fl != -1 && (fl & O_NONBLOCK),
	    "O_NONBLOCK set through the duplicate is not seen on the original");
	pc_pass();
}
