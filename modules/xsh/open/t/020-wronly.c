/*
 * O_WRONLY: writes work, reads fail with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.020
 * MUTANT    open.wronly_readable
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char c;

	pc_begin("XSH.open.020");
	pc_make_file("f", "x");
	pc_test();
	fd = open("f", O_WRONLY);
	pc_expect_ok(fd, "open(\"f\", O_WRONLY)");
	pc_check(write(fd, "y", 1) == 1, "write() failed");
	pc_expect_err(read(fd, &c, 1), EBADF, "read() on an O_WRONLY descriptor");
	pc_check(pc_file_equals("f", "y"), "file does not hold the written data");
	pc_pass();
}
