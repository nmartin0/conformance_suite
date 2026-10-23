/*
 * O_RDWR: both reads and writes work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.021
 * MUTANT    open.rdwr_as_rdonly
 * MUTANT    open.rdwr_as_wronly
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char c;

	pc_begin("XSH.open.021");
	pc_make_file("f", "ab");
	pc_test();
	fd = open("f", O_RDWR);
	pc_expect_ok(fd, "open(\"f\", O_RDWR)");
	pc_check(read(fd, &c, 1) == 1 && c == 'a', "read() failed");
	pc_check(write(fd, "X", 1) == 1, "write() failed");
	pc_check(pc_file_equals("f", "aX"), "file does not hold the written data");
	pc_pass();
}
