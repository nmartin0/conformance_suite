/*
 * O_CREAT on "new/" fails with ENOENT or ENOTDIR and creates nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.044
 * MUTANT    open.trailing_slash_ignored
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	struct stat st;
	int fd, e;

	pc_begin("XSH.open.044");
	pc_require_space();
	pc_test();
	fd = open("n/", O_WRONLY | O_CREAT, 0600);
	e = errno;
	pc_check(fd == -1, "open(\"n/\", O_WRONLY|O_CREAT) succeeded");
	pc_check(e == ENOENT || e == ENOTDIR,
	    "open(\"n/\", O_WRONLY|O_CREAT): errno %s, expected ENOENT or ENOTDIR",
	    pc_errname(e));
	pc_check(lstat("n", &st) != 0, "a file \"n\" was created");
	pc_pass();
}
