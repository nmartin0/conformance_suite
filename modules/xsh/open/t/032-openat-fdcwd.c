/*
 * openat(AT_FDCWD) behaves as open().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.032
 * MUTANT    open.at_fdcwd_ebadf
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char c;

	pc_begin("XSH.open.032");
	pc_make_file("f", "c");
	pc_test();
	fd = openat(AT_FDCWD, "f", O_RDONLY);
	pc_expect_ok(fd, "openat(AT_FDCWD, \"f\", O_RDONLY)");
	pc_check(read(fd, &c, 1) == 1 && c == 'c', "opened the wrong file");
	pc_pass();
}
