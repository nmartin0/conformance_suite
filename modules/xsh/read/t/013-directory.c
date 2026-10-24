/*
 * XSI: read() on a directory works or gives EISDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.013
 * MUTANT    read.eisdir_as_einval
 * VARIANT   read.dir_readable
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char buf[64];
	int fd, e;
	ssize_t r;

	pc_begin("XSH.read.013");
	pc_make_dir("d");
	fd = open("d", O_RDONLY);
	pc_check(fd >= 0, "cannot open directory d");
	pc_test();
	r = read(fd, buf, sizeof buf);
	e = errno;
	pc_check(r >= 0 || e == EISDIR, "read() on a directory failed with %s, expected EISDIR",
	    pc_errname(e));
	pc_pass();
}
