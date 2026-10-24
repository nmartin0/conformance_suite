/*
 * Opening a directory for writing fails with EISDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.015
 * MUTANT    open.eisdir_as_eacces
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.015");
	pc_make_dir("d");
	pc_test();
	fd = open("d", O_WRONLY);
	pc_expect_err(fd, EISDIR, "open(\"d\", O_WRONLY)");
	fd = open("d", O_RDWR);
	pc_expect_err(fd, EISDIR, "open(\"d\", O_RDWR)");
	pc_pass();
}
