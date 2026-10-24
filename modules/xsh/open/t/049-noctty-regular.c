/*
 * O_NOCTTY on a regular file is ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.049
 * MUTANT    open.noctty_enotty
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char c;

	pc_begin("XSH.open.049");
	pc_make_file("f", "x");
	pc_test();
	fd = open("f", O_RDONLY | O_NOCTTY);
	pc_expect_ok(fd, "open(\"f\", O_RDONLY|O_NOCTTY)");
	pc_check(read(fd, &c, 1) == 1 && c == 'x', "read() failed");
	pc_pass();
}
