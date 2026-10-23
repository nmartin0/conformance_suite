/*
 * "file/" without O_CREAT gives ENOTDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.043
 * MUTANT    open.trailing_slash_ignored
 * MUTANT    open.enotdir_as_enoent
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XSH.open.043");
	pc_make_file("f", "x");
	pc_test();
	pc_expect_err(open("f/", O_RDONLY), ENOTDIR, "open(\"f/\", O_RDONLY)");
	pc_pass();
}
