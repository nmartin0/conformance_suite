/*
 * O_CREAT|O_EXCL on a dangling symbolic link: -1/EEXIST, nothing created.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.004
 * MUTANT    open.excl_ignored
 * MUTANT    open.excl_follows_symlink
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
	struct stat st;
	int fd;

	pc_begin("XSH.open.004");
	pc_make_symlink("target", "link");
	pc_test();
	fd = open("link", O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_expect_err(fd, EEXIST, "open(\"link\", O_WRONLY|O_CREAT|O_EXCL)");
	pc_check(lstat("target", &st) != 0, "the link's target was created");
	pc_pass();
}
