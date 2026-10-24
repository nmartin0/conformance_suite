/*
 * openat() resolves a relative path against its directory descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.033
 * MUTANT    open.at_ignores_fd
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int dfd, fd;
	char c;

	pc_begin("XSH.open.033");
	pc_make_dir("d");
	pc_make_file("d/f", "D");
	pc_make_file("f", "C");
	dfd = open("d", O_RDONLY | O_DIRECTORY);
	pc_check(dfd >= 0, "cannot open directory d");
	pc_test();
	fd = openat(dfd, "f", O_RDONLY);
	pc_expect_ok(fd, "openat(dfd, \"f\", O_RDONLY)");
	pc_check(read(fd, &c, 1) == 1, "read() failed");
	pc_check(c == 'D', "opened the file in the working directory, not in d");
	pc_pass();
}
