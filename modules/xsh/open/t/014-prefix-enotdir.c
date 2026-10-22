/*
 * A path prefix that names a non-directory fails with ENOTDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.014
 * MUTANT    open.enotdir_as_enoent
 *
 * O_CREAT is set so that ENOENT's conditions (missing component without
 * O_CREAT) cannot also apply: "f" exists, so with O_CREAT only ENOTDIR
 * does.
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.014");
	pc_make_file("f", "x");
	pc_test();
	fd = open("f/x", O_WRONLY | O_CREAT, 0600);
	pc_expect_err(fd, ENOTDIR, "open(\"f/x\", O_WRONLY|O_CREAT)");
	pc_pass();
}
