/*
 * O_DIRECTORY on a non-directory fails with ENOTDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.013
 * MUTANT    open.directory_ignored
 * MUTANT    open.enotdir_as_enoent
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.013");
	pc_make_file("f", "x");
	pc_test();
	fd = open("f", O_RDONLY | O_DIRECTORY);
	pc_expect_err(fd, ENOTDIR, "open(\"f\", O_RDONLY|O_DIRECTORY)");
	pc_pass();
}
