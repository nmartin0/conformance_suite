/*
 * O_RDONLY: reads work, writes fail with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.019
 * MUTANT    open.rdonly_writable
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char c;

	pc_begin("XSH.open.019");
	pc_make_file("f", "x");
	pc_test();
	fd = open("f", O_RDONLY);
	pc_expect_ok(fd, "open(\"f\", O_RDONLY)");
	pc_check(read(fd, &c, 1) == 1 && c == 'x', "read() did not return the data");
	pc_expect_err(write(fd, "y", 1), EBADF, "write() on an O_RDONLY descriptor");
	pc_check(pc_file_equals("f", "x"), "file was modified");
	pc_pass();
}
