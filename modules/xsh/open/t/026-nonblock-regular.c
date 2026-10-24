/*
 * O_NONBLOCK on a regular file is accepted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.026
 * MUTANT    open.nonblock_regular_eagain
 * VARIANT   open.accmode_bits
 * VARIANT   open.nonblock_hidden_regular
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char c;

	pc_begin("XSH.open.026");
	pc_make_file("f", "x");
	pc_test();
	fd = open("f", O_RDONLY | O_NONBLOCK);
	pc_expect_ok(fd, "open(\"f\", O_RDONLY|O_NONBLOCK)");
	pc_check(read(fd, &c, 1) == 1 && c == 'x', "read() failed");
	pc_pass();
}
