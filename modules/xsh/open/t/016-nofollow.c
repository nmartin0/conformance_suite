/*
 * O_NOFOLLOW on a symbolic link fails with ELOOP.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.016
 * MUTANT    open.nofollow_ignored
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.016");
	pc_make_file("f", "x");
	pc_make_symlink("f", "link");
	pc_test();
	fd = open("link", O_RDONLY | O_NOFOLLOW);
	pc_expect_err(fd, ELOOP, "open(\"link\", O_RDONLY|O_NOFOLLOW)");
	pc_pass();
}
